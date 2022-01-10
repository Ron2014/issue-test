#!/bin/bash
./proc $(find $1 -type f | xargs cat)