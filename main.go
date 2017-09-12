/*
 * Copyright (c) 2014-2017 Sung Pae <self@sungpae.com>
 */

package main

import (
	"flag"
	"fmt"
	"io/ioutil"
	"os"
	"os/exec"
)

const usage = `Usage:
  powerctl COMMAND

Command:
  suspend,  SUSPEND
  reboot,   REBOOT
  poweroff, POWEROFF
  slock`

const systemctlPath = "/usr/bin/systemctl"

func abort(err error) {
	if err != nil {
		fmt.Fprintln(os.Stderr, err.Error())
	}
	os.Exit(1)
}

func systemctl(cmd ...string) {
	if err := exec.Command("/usr/bin/systemctl", cmd...).Run(); err != nil {
		abort(err)
	}
}

func write(path string, bs []byte) {
	if err := ioutil.WriteFile(path, bs, 0600); err != nil {
		abort(err)
	}
}

func main() {
	flag.Usage = func() { fmt.Fprintln(os.Stderr, usage) }
	flag.Parse()

	if flag.NArg() != 1 {
		flag.Usage()
		os.Exit(1)
	}

	cmd := flag.Arg(0)

	switch cmd {
	case "suspend", "reboot", "poweroff":
		systemctl(cmd)
	case "SUSPEND":
		write("/sys/power/state", []byte{'m', 'e', 'm'})
	case "REBOOT":
		write("/proc/sysrq-trigger", []byte{'b'})
	case "POWEROFF":
		write("/proc/sysrq-trigger", []byte{'o'})
	case "slock":
		systemctl("start", "slock.service")
	default:
		flag.Usage()
		os.Exit(1)
	}
}
