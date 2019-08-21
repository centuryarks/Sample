## Recommended register settings

Implement the appropriate IMX378 register settings in the code for each single board computer.

### Notes
Depending on the performance of the Single Board Computer or MPU, the blanking period may not be in time.
Adjust the Frame Vertical Clock Count and Frame Horizontal Clock Count to set the desired frame rate.
```
    / * Frame Horizontal Clock Count * /
    {0x0342, 0x21}, // LINE_LENGTH_PCK [15: 8]
    {0x0343, 0x34}, // LINE_LENGTH_PCK [7: 0]
    / * Frame Vertical Clock Count * /
    {0x0340, 0x0C}, // FRM_LENGTH_LINES [15: 8]
    {0x0341, 0xDE}, // FRM_LENGTH_LINES [7: 0]
```

### RAW8 support
- 640x480@240fps 2100Mbps 2lane
- 1280x720@120fps 2100Mbps 2lane
- 1920x1080@120fps 2100Mbps 2lane
- 3840x2160@30fps 2100Mbps 2lane
- 4056x3040@30fps 2100Mbps 2lane

### RAW10 support
- 640x480@240fps 2100Mbps 2lane
- 1280x720@120fps 2100Mbps 2lane
- 1920x1080@120fps 2100Mbps 2lane
- 3840x2160@30fps 2100Mbps 2lane
- 4056x3040@30fps 2100Mbps 2lane

### RAW12 support
- 640x480@240fps 2100Mbps 2lane
- 1280x720@120fps 2100Mbps 2lane
- 1920x1080@120fps 2100Mbps 2lane
- 3840x2160@30fps 2100Mbps 2lane
- 4056x3040@25fps 2100Mbps 2lane

### MIPI 4 lanes
- 4056x3040@60fps RAW10 2100Mbps 4lane
- 3840x2160@60fps RAW10 2100Mbps 4lane
- 1920x1080@240fps RAW10 2100Mbps 4lane
- 1280x720@240fps RAW10 2100Mbps 4lane
- 1024x768@346fps RAW10 2100Mbps 4lane
- 640x480@535fps RAW10 2100Mbps 4lane
- 4056x3040@44fps RAW12 2100Mbps 4lane
- 3840x2160@60fps RAW12 2100Mbps 4lane
- 1920x1080@120fps RAW12 2100Mbps 4lane
- 1280x720@176fps RAW12 2100Mbps 4lane
- 1024x768@165fps RAW12 2100Mbps 4lane
- 640x480@256fps RAW12 2100Mbps 4lane

### MIPI speed 1500Mbps/lane
- 3840x2160@ 30fps RAW10 1500Mbps 2lane
- 1920x1080@120fps RAW10 1500Mbps 2lane

### MIPI speed 999Mbps/lane
- 3840x2160@ 23fps RAW10 999Mbps 2lane
- 3840x2160@ 15fps RAW10 999Mbps 2lane
- 1920x1080@ 87fps RAW10 999Mbps 2lane
- 1920x1080@ 60fps RAW10 999Mbps 2lane
- 4056x3040@ 30fps RAW10 999Mbps 4lane
- 3840x2160@ 30fps RAW10 999Mbps 4lane
- 1920x1080@120fps RAW10 999Mbps 4lane
- 1280x720@120fps RAW10 999Mbps 4lane
- 1024x768@240fps RAW10 999Mbps 4lane
- 640x480@480fps RAW10 999Mbps 4lane
- 4056x3040@ 24fps RAW12 999Mbps 4lane
- 3840x2160@ 30fps RAW12 999Mbps 4lane
- 1920x1080@120fps RAW12 999Mbps 4lane
- 1280x720@120fps RAW12 999Mbps 4lane
- 1024x768@165fps RAW12 999Mbps 4lane
- 640x480@256fps RAW12 999Mbps 4lane

#### For questions please contact us
ca-qa@centuryarks.com
