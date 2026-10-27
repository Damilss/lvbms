# LV BMS project overview

> Date: 2026-10-26
> Source: `stm32 project_ lv bms.pdf` (team project doc)

## Overall

This project is involved in the new in-house LV battery we are using this year.
It will be in C++, using FreeRTOS for all the scheduling and parallelization.
Both also use I2C to communicate with their respective ICs and CAN to
communicate with the rest of our network.

This is very important so we do not damage our battery :,(

- **Microcontroller:** STM32H573
- **BMS:** BQ7694204

The board will read battery voltage and current and SOC with SPI, use STM sleep
mode and watchdog timers, and propagate this data over CAN, signaling the LV
battery SOC and whether it's time to swap the battery.

I2C and CAN drivers are already made for you (you will probably have to make
your own though). You will need to make watchdog timer drivers and new sleep
mode drivers; these should be pretty simple, and the STM should wake up from any
NVIC interrupt.

Look to other boards (CAN board/AeroCAN) for example task structure. All boards
have a heartbeat task, along with a send CAN task. For this board, you should be
able to do all SPI interactions in a single task.

## Two main code blocks

### Initial programming of BMS IC

Necessary for ensuring the BMS IC starts up with same settings upon reset.

- Determine NVM registers to program
- Write drivers for OTP cycle

### Polling of LV batt data + interrupt handling of fault detection

- Necessary for sending data to dashboard/logger
  - Track SOC, current, voltage, temps
- Make sure the battery doesn't blow up by sending faults to PDC to make it turn off
- Let PDC know when faults are gone

## References

- [Technical Reference Manual (SLUUBY1B)](https://www.ti.com/lit/ug/sluuby1b/sluuby1b.pdf)
  - Brady: page 84 has Commands and Operation Time
- [BQ76942 Datasheet](https://www.ti.com/lit/ds/symlink/bq76942.pdf)

## BQ76942 specs

| Rating                                  | Catalog                        |
| --------------------------------------- | ------------------------------ |
| Number of series cells (max)            | 10                             |
| Number of series cells (min)            | 3                              |
| Vin (max) (V)                           | 55                             |
| Communication interface                 | HDQ, I2C, SPI                  |
| Device type                             | Cell monitor and balancer      |
| Operating temperature range (°C)        | -40 to 85                      |
| Battery overvoltage protection (min) (V) | 1                             |
| Battery overvoltage protection (max) (V) | 5.5                           |
| Operating current (typ) (µA)            | 250                            |

**Features:** digital output, high-side driver, I2C support, integrated ADC, LDO
output, multi-cell support, one-time-programmable (OTP) memory, open-wire
detection, overcurrent in charging protection, overcurrent in discharging
protection, overtemperature protection, overvoltage protection, programmable
EEPROM or EPROM, scratchpad memory, short-current detection, temperature
sensing, undertemperature protection, undervoltage protection.

- Maximum voltage in is 55 V
- Communicates via SPI, I2C, HDQ
- Op current = 250 × 10⁻⁶ A

## Registers

| Register(s)                     | Reading                                     | Type / units            |
| ------------------------------- | ------------------------------------------- | ----------------------- |
| `0x14–0x15` through `0x26–0x27` | Cell 1–10                                   | `uint16_t`, mV          |
| `0x34–0x35`                     | Stack voltage                               | `uint16_t`, µV          |
| `0x36–0x37`                     | PACK pin voltage                            | `uint16_t`, µV          |
| `0x3A–0x3B`                     | CC2 current                                 | `int16_t`, µA           |
| `0x68–0x69`                     | Internal temperature                        | `uint16_t`, 0.1 K       |
| `0x6A–0x7A`                     | External temperatures / configured ADC inputs | `uint16_t`, 0.1 K or mV |
| `0x7F`                          | FET status                                  | one-byte bitfield       |

## Working notes

### Reading a register (AeroCAN C++ as reference)

I'm trying to make sense of C++ code on AeroCAN to figure out how it should be
implemented in our case but I'm not sure yet.

- `addr` is `0x11` because chat says that is the read address. IDK if it is correct.
- `memAddr` is `0x14` because that is the address of the first bit for the
  battery voltage of cell 1.
- The size of the register address itself is 1 byte, so `memAddrSize` is `1`.
- The data pointer is passed in to get populated.
- `size` is `2` because the data is a 2-byte (16-bit) unsigned integer.

### Brady: SPI protocol

**We request data in frame n. We can only receive the response in frame n+1.**

- We need to have a blank packet to receive the final requested data
  (1 "wasted" transmit).
- Each request for cell voltage requires us to wait at least **50 µs** before
  sending another request.

### Emilio: SPI protocol

> "The first byte of a SPI transaction consists of an R/W bit (R=0, W=1),
> followed by a 7-bit address, **MSB-first**. If the controller (host) is
> writing, then the second byte will be the data to be written. If the
> controller is reading, then the second byte sent on SPI_MOSI is ignored
> (except for CRC calculation)"
