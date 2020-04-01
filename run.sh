#!/bin/bash
cargo bootimage || exit 1
qemu-system-x86_64 -drive format=raw,file=target/x86_64-tos/debug/bootimage-tos.bin
