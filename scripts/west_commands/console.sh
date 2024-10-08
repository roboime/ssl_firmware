#!/bin/bash
#
# Script to set up a tmux session for monitoring logs and handling commands
# for the Nucleo H723ZG board.
#
set -eu

# Define your serial device and baud rate
SERIAL_DEVICE="/dev/ttyACM0" # Adjust this to your actual serial device
BAUD_RATE="115200"
LOG_FILE="$HOME/zephyr.log" # Path to the log file
COMMAND_PIPE="/tmp/zephyr_command_pipe"

# Function to disconnect all processes using the serial device
disconnect_serial_device() {
    echo "Disconnecting all processes using $SERIAL_DEVICE..."
    lsof -t "$SERIAL_DEVICE" | xargs -r sudo kill -9
    
    LOCK_FILE="/var/lock/LCK..$(basename "$SERIAL_DEVICE")"
    if [ -e "$LOCK_FILE" ]; then
        echo "Removing lock file $LOCK_FILE"
        sudo rm "$LOCK_FILE"
    fi
}

# Function to create a new tmux session
create_tmux_session() {
    # Create the named pipe if it doesn't exist
    if [[ ! -p $COMMAND_PIPE ]]; then
        mkfifo $COMMAND_PIPE
    fi

    tmux new-session -d -s zephyr_session \; \
        send-keys "clear && stty -F $SERIAL_DEVICE $BAUD_RATE raw -echo && cat $COMMAND_PIPE > $SERIAL_DEVICE & cat $SERIAL_DEVICE | tee $LOG_FILE | sed -u -e 's/<dbg>/\x1b[35m&\x1b[1000m/g' -e 's/<inf>/\x1b[32m&\x1b[1000m/g'" C-m \; \
        split-window -h \; \
        send-keys "clear && cat > $COMMAND_PIPE" C-m \; \
        resize-pane -D 10 \; \
        select-pane -t 0 \; \
        set -g status-style bg=blue,fg=white \; \
        set -g status-right-length 200 \; \
        set -g status-left-length 500 \; \
        set -g status-left 'RoboIME Console | CTRL + q (exit) | CTRL + h (switch console) |' \; \
        set -g status-right "| Serial: $SERIAL_DEVICE | Bound Rate: $BAUD_RATE |" \; \
        bind-key -n F10 kill-session \; \
        bind-key -n F9 select-pane -L \; \
        set -g mouse on \; \
        attach-session -t zephyr_session

}

# Main script execution
disconnect_serial_device

if tmux has-session -t zephyr_session 2>/dev/null; then
    echo "Attaching to existing tmux session 'zephyr_session'."
    tmux attach-session -t zephyr_session
else
    echo "Creating new tmux session 'zephyr_session'."
    create_tmux_session
fi

# Clean up the named pipe when the script exits
trap "rm -f $COMMAND_PIPE" EXIT
