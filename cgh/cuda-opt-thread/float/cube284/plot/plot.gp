set grid           # 目盛線を描画することを指定
set xlabel "thread.y"  # x軸の見出しを指定
set ylabel "milisecond"           # 第一y軸の見出しを指定
# 凡例
set key top left   # 凡例は左上に描画
plot 'thread_x_1.txt' title 'thread.x=1' lw 1 with lines,\
   'thread_x_2.txt' title 'thread.x=2' lw 1 with lines,\
   'thread_x_3.txt' title 'thread.x=3' lw 1 with lines,\
   'thread_x_5.txt' title 'thread.x=5' lw 1 with lines,\
   'thread_x_10.txt' title 'thread.x=10' lw 1 with lines,\
	; pause -1"
