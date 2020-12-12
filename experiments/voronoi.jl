
function voronoi(x, y, n)
    points = collect(zip(Int.(floor.(x * rand(n))), Int.(floor.(y * rand(n)))))
    image = Array{Float32}(undef, x, y)
    for i = 1:y
        for j = 1:x
            image[i, j] = minimum(p -> eucdist((j, i), p), points)
        end
    end
    image / max(image...)
end
mandist((x1, y1), (x2, y2)) = abs(x1 - x2) + abs(y1 - y2)
eucdist((x1, y1), (x2, y2)) = sqrt((x1 - x2)^2 + (y1 - y2)^2)
