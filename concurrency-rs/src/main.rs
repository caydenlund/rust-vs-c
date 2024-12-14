use std::env;
use std::sync::Arc;
use std::thread;
use std::time::Instant;

// Structure to pass arguments to each thread
struct ThreadData {
    id: usize,
    num_threads: usize,
    dim: usize,
    mat_a: Arc<Vec<f32>>,
    mat_b: Arc<Vec<f32>>,
    mat_c: Arc<Vec<f32>>,
}

fn mat_mul_thread(data: ThreadData) {
    let dim = data.dim;
    let chunk_size = dim / data.num_threads;
    let start_row = data.id * chunk_size;
    let end_row = if data.id == data.num_threads - 1 {
        dim
    } else {
        start_row + chunk_size
    };

    let mut mat_c = data.mat_c.as_ref().to_vec(); // Clone to modify

    for i in start_row..end_row {
        for j in 0..dim {
            let mut sum = 0.0;
            for k in 0..dim {
                sum += data.mat_a[i * dim + k] * data.mat_b[k * dim + j];
            }
            mat_c[i * dim + j] = sum;
        }
    }
}

fn print_usage(program: &str) {
    println!(
        "Usage: {} -t <num_threads> -n <mat_dim> -r <num_repetitions>",
        program
    );
}

fn main() -> Result<(), ()> {
    let mut num_threads = 0;
    let mut mat_dim = 0;
    let mut num_repetitions = 0;

    let mut args = env::args();
    let prgm = args.next().unwrap();
    while let Some(arg) = args.next() {
        match arg.as_str() {
            "-t" => {
                let Some(arg) = args.next() else {
                    print_usage(&prgm);
                    return Err(());
                };
                num_threads = arg.parse().unwrap_or(0);
            }
            "-n" => {
                let Some(arg) = args.next() else {
                    print_usage(&prgm);
                    return Err(());
                };
                mat_dim = arg.parse().unwrap_or(0);
            }
            "-r" => {
                let Some(arg) = args.next() else {
                    print_usage(&prgm);
                    return Err(());
                };
                num_repetitions = arg.parse().unwrap_or(0);
            }
            _ => {}
        }
    }

    if num_threads == 0 || mat_dim == 0 || num_repetitions == 0 {
        print_usage(&prgm);
        return Err(());
    }

    let mut mat_a = vec![0.0f32; mat_dim * mat_dim];
    for (ind, item) in mat_a.iter_mut().enumerate() {
        *item = 1.0 / ((ind + 1) * (ind + 2)) as f32;
    }
    let mat_a = Arc::new(mat_a);

    let mut mat_b = vec![0.0f32; mat_dim * mat_dim];
    for (ind, item) in mat_b.iter_mut().enumerate() {
        *item = (ind + 3) as f32;
    }
    let mat_b = Arc::new(mat_b);

    let mat_c = Arc::new(vec![0.0f32; mat_dim * mat_dim]);

    let start_time = Instant::now();

    for _ in 0..num_repetitions {
        let mut handles = vec![];

        for id in 0..num_threads {
            let thread_data = ThreadData {
                id,
                num_threads,
                dim: mat_dim,
                mat_a: Arc::clone(&mat_a),
                mat_b: Arc::clone(&mat_b),
                mat_c: Arc::clone(&mat_c),
            };

            let handle = thread::spawn(move || {
                mat_mul_thread(thread_data);
            });

            handles.push(handle);
        }

        for handle in handles {
            handle.join().unwrap();
        }
    }

    let total_time = start_time.elapsed().as_nanos();
    let time_per_iter = total_time / num_repetitions;

    println!("{}\t{}\t{}", num_threads, mat_dim, time_per_iter);

    Ok(())
}
