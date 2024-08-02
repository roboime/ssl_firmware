#!/bin/sh

#
# Script to set up a tmux session for monitoring logs and handling commands
# for the Nucleo H723ZG board.
#

set -eu

# Define your serial device and baud rate
SERIAL_DEVICE="/dev/ttyACM0"  # Adjust this to your actual serial device
BAUD_RATE="115200"
LOG_FILE="$HOME/zephyr.log"   # Path to the log file

# Function to disconnect all processes using the serial device
disconnect_serial_device() {
    echo "Disconnecting all processes using $SERIAL_DEVICE..."
    
    # Find and kill all processes using the serial device
    for pid in $(lsof -t $SERIAL_DEVICE); do
        echo "Killing process $pid"
        sudo kill -9 $pid
    done
    
    # Check for and remove any lock files
    LOCK_FILE="/var/lock/LCK..$(basename $SERIAL_DEVICE)"
    if [ -e "$LOCK_FILE" ]; then
        echo "Removing lock file $LOCK_FILE"
        sudo rm "$LOCK_FILE"
    fi
}

# Function to create a new tmux session
create_tmux_session() {
    tmux new-session -d -s zephyr_session \; \
      bind-key -n C-q kill-session \; \
      send-keys "screen $SERIAL_DEVICE $BAUD_RATE" C-m \; \
      split-window -h \; \
      send-keys "tail -f $LOG_FILE 2>/dev/null" C-m \; \
      resize-pane -R 10 \; \
      split-window -v \; \
      send-keys 'while true; do nc localhost 19021; sleep 0.5; done' C-m \; \
      resize-pane -D 10 \; \
      select-pane -t 0 \; \
      bind -n C-h select-pane -L
      attach-session -t zephyr_session
}

# Disconnect any existing connections to the serial device
disconnect_serial_device

# Check if the tmux session already exists
if tmux has-session -t zephyr_session 2>/dev/null; then
    echo "Attaching to existing tmux session 'zephyr_session'."
    tmux attach-session -t zephyr_session
else
    echo "Creating new tmux session 'zephyr_session'."
    create_tmux_session
fi
