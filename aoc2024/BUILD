days = [
    "day01", "day02", "day03", "day04", "day05", "day06", "day07", "day08", "day09", "day10",
]

[
    cc_binary(
        name = day,
        srcs = [day + ".cpp"],
        deps = [
            "//tools",
            "@abseil-cpp//absl/container:flat_hash_map",
            "@abseil-cpp//absl/strings",
        ]
    )
    for day in days
]
