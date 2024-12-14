use std::env;
use std::time::Instant;

fn insertion_sort(arr: &mut [u64]) {
    for r_ind in 1..arr.len() {
        let key = arr[r_ind];
        let mut l_ind = r_ind as isize - 1;

        while l_ind >= 0 && arr[l_ind as usize] > key {
            arr[(l_ind + 1) as usize] = arr[l_ind as usize];
            l_ind -= 1;
        }

        arr[(l_ind + 1) as usize] = key;
    }
}

fn merge_sort(arr: &mut [u64]) {
    if arr.len() <= 1 {
        return;
    }

    let mid = arr.len() / 2;
    let (left, right) = arr.split_at_mut(mid);

    merge_sort(left);
    merge_sort(right);

    merge(arr, mid);
}

fn merge(arr: &mut [u64], mid: usize) {
    let left = arr[..mid].to_vec();
    let right = arr[mid..].to_vec();

    let mut left_ind = 0;
    let mut right_ind = 0;
    let mut curr_ind = 0;

    while left_ind < left.len() && right_ind < right.len() {
        if left[left_ind] <= right[right_ind] {
            arr[curr_ind] = left[left_ind];
            left_ind += 1;
        } else {
            arr[curr_ind] = right[right_ind];
            right_ind += 1;
        }
        curr_ind += 1;
    }

    while left_ind < left.len() {
        arr[curr_ind] = left[left_ind];
        left_ind += 1;
        curr_ind += 1;
    }

    while right_ind < right.len() {
        arr[curr_ind] = right[right_ind];
        right_ind += 1;
        curr_ind += 1;
    }
}

fn radix_sort(arr: &mut [u64]) {
    if arr.len() <= 1 {
        return;
    }

    let mut temp = vec![0; arr.len()];

    for shift in (0..64).step_by(8) {
        let mut counts = [0; 256];

        for &num in arr.iter() {
            let byte = ((num >> shift) & 0xFF) as usize;
            counts[byte] += 1;
        }

        for i in 1..256 {
            counts[i] += counts[i - 1];
        }

        for &num in arr.iter().rev() {
            let byte = ((num >> shift) & 0xFF) as usize;
            let index = counts[byte] - 1;
            counts[byte] -= 1;
            temp[index] = num;
        }

        arr.copy_from_slice(&temp);
    }
}

fn validate(arr: &[u64]) {
    for window in arr.windows(2) {
        if window[0] > window[1] {
            eprintln!("Error: invalid sort");
            std::process::exit(1);
        }
    }
}

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
    let (num_elems, num_repetitions) = {
        let mut num_elems = 0;
        let mut num_repetitions = 0;

        let mut args = env::args();
        let prgm_name = args.next().unwrap();
        while let Some(arg) = args.next() {
            match arg.as_str() {
                "-n" => {
                    if let Some(dim) = args.next() {
                        num_elems = dim.parse::<usize>().unwrap();
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

        if num_elems == 0 || num_repetitions == 0 {
            print_usage(&prgm_name);
            return;
        }

        (num_elems, num_repetitions)
    };

    let mut total_insertion_time = 0;
    let mut total_merge_time = 0;
    let mut total_radix_time = 0;
    let mut total_validate_time = 0;

    let mut val = rand_next(0);

    for _ in 0..num_repetitions {
        let elems: Vec<u64> = (0..num_elems)
            .map(|_| {
                val = rand_next(val);
                val
            })
            .collect();
        let mut elems_insertion_clone = elems.clone();
        let mut elems_merge_clone = elems.clone();
        let mut elems_radix_clone = elems.clone();

        let start = Instant::now();
        insertion_sort(&mut elems_insertion_clone);
        let end = Instant::now();
        total_insertion_time += time_diff_ns(start, end);

        let start = Instant::now();
        merge_sort(&mut elems_merge_clone);
        let end = Instant::now();
        total_merge_time += time_diff_ns(start, end);

        let start = Instant::now();
        radix_sort(&mut elems_radix_clone);
        let end = Instant::now();
        total_radix_time += time_diff_ns(start, end);

        let start = Instant::now();
        validate(&elems_insertion_clone);
        validate(&elems_merge_clone);
        validate(&elems_radix_clone);
        let end = Instant::now();
        total_validate_time += time_diff_ns(start, end);
    }

    println!(
        "{}\t{}\t{}\t{}\t{}",
        num_elems,
        total_insertion_time / num_repetitions,
        total_merge_time / num_repetitions,
        total_radix_time / num_repetitions,
        total_validate_time / (num_repetitions * 3)
    );
}
