["seeds: " <> seeds_str | maps_str] =
    "inp/day5.txt"
    |> File.read!()
    |> String.split("\n\n")

seeds =
    seeds_str
    |> String.split(" ")
    |> Enum.map(&String.to_integer/1)
    |> Enum.chunk_every(2)
    |> Enum.map(&List.to_tuple/1)

maps =
    maps_str
    |> Enum.map(fn ln ->
        String.split(ln, "\n", trim: true)
        |> tl()
        |> Enum.map(fn stuff ->
            String.split(stuff, " ")
            |> Enum.map(&String.to_integer/1)
            |> List.to_tuple()
        end)
    end)

Enum.reduce(
    maps,
    seeds,
    fn map, ranges ->
        for range <- ranges,
            new_ranges <- Enum.reduce(
                map,
                {[], [range]},
                fn {destination_start, source_start, map_length}, {found, [{range_start, range_length} | remaining] = all_remaining} ->
                    overlap_start = max(range_start, source_start)
                    overlap_end = min(range_start + range_length - 1, source_start + map_length - 1)
                    overlap_length = overlap_end - overlap_start + 1
                    if overlap_length > 0 do
                        move = destination_start - source_start
                        new_range = {overlap_start + move, overlap_length}
                        residual1 = if overlap_start > range_start do
                            [{range_start, overlap_start - range_start}]
                        else
                            []
                        end
                        residual2 = if overlap_end < range_start + range_length - 1 do
                            [{overlap_end + 1, range_start + range_length - 1 - overlap_end}]
                        else
                            []
                        end
                        {[new_range | found], residual1 ++ residual2 ++ remaining}
                    else
                        {found, all_remaining}
                    end
                    _, {found, []} -> {found, []}
                end
            )
            |> then(fn {found, not_found} -> found ++ not_found end)
        do
        new_ranges
        end
    end
)
|> Enum.map(& elem(&1, 0))
|> Enum.min()
|> IO.inspect()
