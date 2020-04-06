#!/bin/bash
pcompile prog1.s && ldpic32 prog1.hex && pterm
echo "Started"
