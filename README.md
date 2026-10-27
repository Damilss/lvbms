# lvbms

> This project is for the Firmware Calpoly FSAE subsystem see [Cal Poly FSAE](https://github.com/calpolyfsae/)

## objective 

Read and Write functions/drivers for our microcontroller STM32H573 and our bms BQ7694204

## repository structure
```sh
.
├── boards
│   ├── LVBattery
│   │   └── source
│   │       ├── clocks.h
│   │       ├── HAL
│   │       │   ├── FreeRTOSConfig.h
│   │       │   ├── stm32h5xx_hal_conf.h
│   │       │   ├── stm32h5xx_it.cpp
│   │       │   └── stm32h5xx_it.h
│   │       ├── main.cpp
│   │       ├── main.h
│   │       ├── pins.h
│   │       ├── taskmanager.cpp
│   │       └── taskmanager.h
│   └── LVM
├── README.md
├── scripts
└── shared

8 directories, 11 files
```

## New members 

**PLEASE SEE [CONTRIBUTING.md](CONTRIBUTING.md)**

