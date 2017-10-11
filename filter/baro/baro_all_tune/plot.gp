set ytics nomirror
set y2tics         # 第二y軸を描画することを指定
set grid           # 目盛線を描画することを指定
set xlabel "microsec"  # x軸の見出しを指定
set ylabel "alt"           # 第一y軸の見出しを指定
set y2label "vel and acc" # 第二y軸の見出しを指定
# 凡例
set key top left   # 凡例は左上に描画
plot 'out5mbaroraw.txt' title 'raw' lw 1 with lines axis x1y1,\
	'all.txt' using 1:2 title 'alt' lw 3 with lines axis x1y1,\
	'all.txt' using 1:3 title 'vel' lw 3 with lines axis x1y2, \
	'all.txt' using 1:4 title 'acc' lw 3 with lines axis x1y2 \
	; pause -1"
