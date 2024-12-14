use std::env;
use std::time::Instant;

fn time_diff_ns(start: Instant, end: Instant) -> u64 {
    end.duration_since(start).as_nanos() as u64
}

fn rand_next(seed: u64) -> u64 {
    const MODULUS: u64 = 1 << 31;
    const MULTIPLIER: u64 = 1103515245;
    const INCREMENT: u64 = 12345;

    (seed * MULTIPLIER + INCREMENT) % MODULUS
}

fn print_usage(prgm_name: &str) {
    println!("Usage: {} -n <mat_dim> -r <num_repetitions>", prgm_name);
}

fn main() {
    let (mat_dim, num_repetitions) = {
        let mut mat_dim = 0;
        let mut num_repetitions = 0;

        let mut args = env::args();
        let prgm_name = args.next().unwrap();
        while let Some(arg) = args.next() {
            match arg.as_str() {
                "-n" => {
                    if let Some(dim) = args.next() {
                        mat_dim = dim.parse::<usize>().unwrap();
                    } else {
                        print_usage(&prgm_name);
                        return;
                    }
                }
                "-r" => {
                    if let Some(repetitions) = args.next() {
                        num_repetitions = repetitions.parse::<u64>().unwrap();
                    } else {
                        print_usage(&prgm_name);
                        return;
                    }
                }
                _ => {
                    print_usage(&prgm_name);
                    return;
                }
            }
        }

        if mat_dim == 0 || num_repetitions == 0 {
            print_usage(&prgm_name);
            return;
        }

        (mat_dim, num_repetitions)
    };

    let mut total_time = 0;
    let mut num = rand_next(0);

    for _ in 0..num_repetitions {
        let mut mat_a = vec![0.0f32; mat_dim * mat_dim];
        let mut mat_b = vec![0.0f32; mat_dim * mat_dim];
        let mut mat_c = vec![0.0f32; mat_dim * mat_dim];

        for ind in 0..(mat_dim * mat_dim) {
            num = rand_next(num);
            mat_a[ind] = num as f32;
            num = rand_next(num);
            mat_b[ind] = num as f32;
        }

        let start = Instant::now();

        for i in 0..mat_dim {
            for j in 0..mat_dim {
                mat_c[i * mat_dim + j] = 0.0;
                for k in 0..mat_dim {
                    mat_c[i * mat_dim + j] += mat_a[i * mat_dim + k] * mat_b[k * mat_dim + j];
                }
            }
        }

        let end = Instant::now();
        total_time += time_diff_ns(start, end);
    }

    println!("{}\t{}", mat_dim, total_time / num_repetitions);
}
