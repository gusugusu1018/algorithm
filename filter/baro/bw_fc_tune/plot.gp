set ytics nomirror
set grid           # 目盛線を描画することを指定
set xlabel "microsec"  # x軸の見出しを指定
set ylabel "alt"           # 第一y軸の見出しを指定
# 凡例
set key top left   # 凡例は左上に描画
plot 'all.txt' using 1:2 title 'raw' with lines,\
		'all.txt' using 1:3 title '0.1' with lines lw 2,\
		'all.txt' using 1:4 title '0.5' with lines lw 2, \
		'all.txt' using 1:5 title '1.0' with lines lw 2, \
		'all.txt' using 1:6 title '2.0' with lines lw 2, \
		'all.txt' using 1:7 title '3.0' with lines lw 2, \
		'all.txt' using 1:8 title '4.0' with lines lw 2, \
		'all.txt' using 1:9 title '5.0' with lines lw 2, \
		'all.txt' using 1:10 title '10.0' with lines lw 2, \
		; pause -1"
