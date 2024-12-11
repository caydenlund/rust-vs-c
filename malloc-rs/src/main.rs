use std::time::{Duration, Instant};

const MIN_ALLOC_SIZE: u64 = 8;
const MAX_ALLOC_SIZE: u64 = 1024;

#[derive(Debug, Clone)]
struct MemoryBenchmarkResult {
    total_alloc_time: Duration,
    total_dealloc_time: Duration,
    total_memory_allocated: usize,
}

fn rand(seed: u64) -> u64 {
    const MODULUS: u64 = 1 << 31;
    const MULTIPLIER: u64 = 1103515245;
    const INCREMENT: u64 = 12345;

    (seed * MULTIPLIER + INCREMENT) % MODULUS
}

fn benchmark_random_allocs(alloc_count: usize) -> MemoryBenchmarkResult {
    let mut allocations: Vec<Vec<u8>> = Vec::with_capacity(alloc_count);
    let mut total_memory_allocated = 0;

    let mut size = rand(0);
    let allocation_start = Instant::now();
    for _ in 0..alloc_count {
        size = MIN_ALLOC_SIZE + rand(size) % (MAX_ALLOC_SIZE - MIN_ALLOC_SIZE);
        total_memory_allocated += size as usize;
        allocations.push(vec![0; size as usize]);
    }
    let total_alloc_time = allocation_start.elapsed();

    let deallocation_start = Instant::now();
    drop(allocations);
    let total_dealloc_time = deallocation_start.elapsed();

    MemoryBenchmarkResult {
        total_alloc_time,
        total_dealloc_time,
        total_memory_allocated,
    }
}

fn main() {
    let mut args = std::env::args();
    args.next();
    let alloc_count = args.next().unwrap().parse().unwrap();

    let MemoryBenchmarkResult {
        total_alloc_time,
        total_dealloc_time,
        total_memory_allocated,
    } = benchmark_random_allocs(alloc_count);

    println!(
        r"Total allocation time:   {} ns
Total deallocation time: {} ns
Total memory allocated:  {} B
Total allocations:       {}",
        total_alloc_time.as_nanos(),
        total_dealloc_time.as_nanos(),
        total_memory_allocated,
        alloc_count,
    )
}
