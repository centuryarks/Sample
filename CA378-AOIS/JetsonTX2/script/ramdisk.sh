#!/bin/sh
if [ -e /mnt/ram ]; then
  sudo umount /mnt/ram
  sudo rm -r /mnt/ram
fi
sudo mkdir /mnt/ram
sudo mount -t tmpfs -o size=4G /dev/shm /mnt/ram
