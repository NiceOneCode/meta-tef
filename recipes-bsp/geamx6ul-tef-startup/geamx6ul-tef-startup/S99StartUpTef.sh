echo "Engicam tef startup"

#####disable framebuffer blank timeout
echo 0 > /sys/class/graphics/fbcon/cursor_blink
echo -e '\033[9;0]' > /dev/tty1  
