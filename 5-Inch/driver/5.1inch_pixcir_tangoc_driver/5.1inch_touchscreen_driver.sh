#/bin/bash
rmmod pixcir_hid_tangoc.ko
echo "Start install 5 point multi_touch panel driver......"
make clean
sleep 1s
make 
sleep 1s
insmod pixcir_hid_tangoc.ko
echo "install driver successfully"
