use std::env;
use std::time::{Duration, Instant};

const MIN_ALLOC_SIZE: u64 = 8;
const MAX_ALLOC_SIZE: u64 = 1024;

fn rand_next(seed: u64) -> u64 {
    const MODULUS: u64 = 1 << 31;
    const MULTIPLIER: u64 = 1103515245;
    const INCREMENT: u64 = 12345;

    (seed * MULTIPLIER + INCREMENT) % MODULUS
}

fn print_usage(program: &str) {
    eprintln!("Usage: `{} -n <num_allocs> -r <num_repetitions>`", program);
}

fn main() {
    let args: Vec<String> = env::args().collect();
    let mut num_allocs = 0;
    let mut num_repetitions = 0;

    let mut arg_iter = args.iter().skip(1);
    while let Some(arg) = arg_iter.next() {
        match arg.as_str() {
            "-n" => {
                if let Some(value) = arg_iter.next() {
                    num_allocs = value.parse().unwrap_or(0);
                } else {
                    print_usage(&args[0]);
                    return;
                }
            }
            "-r" => {
                if let Some(value) = arg_iter.next() {
                    num_repetitions = value.parse().unwrap_or(0);
                } else {
                    print_usage(&args[0]);
                    return;
                }
            }
            _ => {
                print_usage(&args[0]);
                return;
            }
        }
    }

    if num_allocs == 0 || num_repetitions == 0 {
        print_usage(&args[0]);
        return;
    }

    let mut total_alloc_time = 0;
    let mut total_free_time = 0;

    let mut size = 0;
    for _ in 0..num_repetitions {
        let alloc_start = Instant::now();
        let mut allocations = Vec::with_capacity(num_allocs as usize);
        for _ in 0..num_allocs {
            size = MIN_ALLOC_SIZE + (rand_next(size) % (MAX_ALLOC_SIZE - MIN_ALLOC_SIZE));

            allocations.push(vec![0; size as usize]);
        }
        total_alloc_time += alloc_start.elapsed().as_nanos();

        let free_start = Instant::now();
        drop(allocations);
        total_free_time += free_start.elapsed().as_nanos();
    }

    println!(
        "{}\t{}",
        total_alloc_time / num_repetitions,
        total_free_time / num_repetitions
    );
}
