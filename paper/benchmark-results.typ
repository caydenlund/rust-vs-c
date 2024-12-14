= Benchmark Results

== `malloc-c` and `malloc-rs`

=== C:
#table(
  columns: (auto, auto, auto),
  table.header(
    [Number of allocations], [Malloc runtime (ns)], [Free runtime (ns)],
  ),
  [10], [282], [149],
  [100], [2087], [1296],
  [1000], [187746], [49918],
  [10000], [2965090], [1407816],
)

=== Rust:
#table(
  columns: (auto, auto, auto),
  table.header(
    [Number of allocations], [Malloc runtime (ns)], [Free runtime (ns)],
  ),
  [10], [456], [171],
  [100], [5031], [1488],
  [1000], [782390], [160985],
  [10000], [10270961], [2961104],
)

== `matrix-c` and `matrix-rs`

=== C:
#table(
  columns: (auto, auto, auto),
  table.header(
    [Matrix dim.], [Runtime (ns)],
  ),
  [40], [33537],
  [80], [281912],
  [160], [2807064],
  [320], [23935532],
)

=== Rust:
#table(
  columns: (auto, auto, auto),
  table.header(
    [Matrix dim.], [Runtime (ns)],
  ),
  [40], [49020],
  [80], [413897],
  [160], [3187199],
  [320], [25249436],
)

== `sort-c` and `sort-rs`

=== C:
#table(
  columns: (auto, auto, auto),
  table.header(
    [Array length], [Insertion sort runtime (ns)], [Merge sort runtime (ns)], [Radix sort runtime (ns)], [Validation runtime (ns)],
  ),
  [10], [168], [688], [1843], [21],
  [100], [2963], [8984], [5295], [126],
  [1000], [135559], [96818], [39920], [982],
  [10000], [14868382], [1112055], [392420], [9463],
)

=== Rust:
#table(
  columns: (auto, auto, auto),
  table.header(
    [Array length], [Insertion sort runtime (ns)], [Merge sort runtime (ns)], [Radix sort runtime (ns)], [Validation runtime (ns)],
  ),
  [10], [116], [490], [1478], [16],
  [100], [1688], [5516], [3155], [55],
  [1000], [106577], [68256], [21687], [452],
  [10000], [9510456], [813836], [221366], [4489],
)

== `concurrency-c` and `concurrency-rs`

=== C:
#table(
  columns: (auto, auto, auto),
  table.header(
    [Number of threads], [Matrix size], [Runtime (ns)],
  ),
  [1], [10], [41852],
  [2], [10], [54338],
  [4], [10], [92583],
  [6], [10], [183286],
  [8], [10], [227091],
  [1], [100], [657328],
  [2], [100], [400184],
  [4], [100], [251194],
  [6], [100], [264599],
  [8], [100], [286981],
  [1], [400], [46047054],
  [2], [400], [24367671],
  [4], [400], [12156049],
  [6], [400], [9415956],
  [8], [400], [9658438],
)

=== Rust:
#table(
  columns: (auto, auto, auto),
  table.header(
    [Number of threads], [Matrix size], [Runtime (ns)],
  ),
  [1], [10], [39372],
  [2], [10], [55271],
  [4], [10], [85267],
  [6], [10], [133467],
  [8], [10], [162697],
  [1], [100], [923054],
  [2], [100], [552538],
  [4], [100], [307088],
  [6], [100], [309694],
  [8], [100], [313766],
  [1], [400], [51287927],
  [2], [400], [26605527],
  [4], [400], [13590651],
  [6], [400], [11203079],
  [8], [400], [13722652],
)
