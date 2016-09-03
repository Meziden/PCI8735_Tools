set terminal png size 1080,720
set output "Result.png"
set title "Testing"
set xlabel "Speed/RPM"
set ylabel "Value"
plot "./testdata_statis.tdf" using 1:2 smooth bezier,"testdata_statis.tdf" using 1:3 smooth bezier,"testdata_statis.tdf" using 1:4 smooth bezier,"testdata_statis.tdf" using 1:5 smooth bezier
