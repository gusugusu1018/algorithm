set ytics nomirror
set grid           # 目盛線を描画することを指定
set xlabel "microsec"  # x軸の見出しを指定
set ylabel "alt"           # 第一y軸の見出しを指定
# 凡例
set key top left   # 凡例は左上に描画
plot 'alt.txt' using 1:2 title 'alt' lw 1 with lines,\
	'alt.txt' using 1:3 title 'Butter Worth Filter' lw 2 with lines,\
	; pause -1"
