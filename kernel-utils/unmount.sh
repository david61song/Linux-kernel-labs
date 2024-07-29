#!/bin/bash

# Set the mount point
MOUNT_POINT="$(pwd)/rootfs"

# Check if rootfs is mounted
if mount | grep -q "$MOUNT_POINT"; then
    echo "Unmounting $MOUNT_POINT..."
    
    # Try to unmount normally first
    sudo umount "$MOUNT_POINT"
    
    # Check if unmount was successful
    if mount | grep -q "$MOUNT_POINT"; then
        echo "Normal unmount failed. Checking for processes using the mount point..."
        
        # Check and display processes using the mount point
        lsof "$MOUNT_POINT"
        
        # Ask user if they want to terminate these processes
        read -p "Do you want to terminate these processes? (y/n) " answer
        if [[ $answer == "y" ]]; then
            # Force terminate the processes
            lsof "$MOUNT_POINT" | awk 'NR!=1 {print $2}' | xargs sudo kill -9
            echo "Processes terminated."
            
            # Try to unmount again
            sudo umount "$MOUNT_POINT"
        else
            echo "Processes not terminated. Trying force unmount..."
            # Try force unmount
            sudo umount -f "$MOUNT_POINT"
        fi
    fi
    
    # Final check
    if mount | grep -q "$MOUNT_POINT"; then
        echo "Failed to unmount $MOUNT_POINT"
    else
        echo "$MOUNT_POINT successfully unmounted"
    fi
else
    echo "$MOUNT_POINT is not mounted"
fi
