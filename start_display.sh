#!/bin/bash

# Open the first Firefox instance and wait for it to start
firefox --new-window --kiosk "http://localhost:5173/create-post" &
sleep 3  # Adjust the sleep duration as needed to allow Firefox to open

# Move the first Firefox instance to the first monitor and make it fullscreen
firefox_window_id=$(wmctrl -l | grep "Mozilla Firefox" | awk '{print $1}')
xdotool windowmove $firefox_window_id 0 1080
xdotool windowsize $firefox_window_id 1920 1080  # Adjust the resolution as needed

# Open the second Firefox instance on the second monitor and make it fullscreen
firefox --new-window --kiosk "http://localhost:5173/" &
sleep 3  # Adjust the sleep duration as needed

# Move the second Firefox instance to the second monitor and make it fullscreen
firefox_window_id=$(wmctrl -l | grep "Mozilla Firefox" | awk '{print $1}' | tail -n 1)
xdotool windowmove $firefox_window_id 0 0
xdotool windowsize $firefox_window_id 1920 1080  # Adjust the resolution as needed
