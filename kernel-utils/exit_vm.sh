#!/bin/bash

sudo kill -s SIGINT $(sudo cat vm.pid)
