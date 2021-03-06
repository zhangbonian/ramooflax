/*
** Copyright (C) 2011 EADS France, stephane duverger <stephane.duverger@eads.net>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License along
** with this program; if not, write to the Free Software Foundation, Inc.,
** 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#include <vmm.h>
#include <vm.h>
#include <mp.h>
#include <dev_kbd.h>
#include <dev_pic.h>
#include <dev_uart.h>
#include <dev_io_ports.h>

#include <debug.h>
#include <info_data.h>

extern info_data_t *info;

static void vmm_dev_init()
{
   info->vm.dev.mem.a20 = 1;

   dev_kbd_init(&info->vm.dev.kbd);
   dev_uart_init(&info->vm.dev.uart, SERIAL_COM1);

   /* proxify to detect rebase for uart irq injection */
   info->vm.dev.pic1_icw2 = DFLT_PIC1_ICW2;
   __deny_io_range(PIC1_START_PORT, PIC1_END_PORT);

   /* lazzy emulation */
   __deny_io_range(COM1_START_PORT, COM1_END_PORT);

   /* monitor reboot and A20 */
   __deny_io_range(KBD_START_PORT, KBD_END_PORT);
   __deny_io(PS2_SYS_CTRL_PORT_A);
}

void __regparm__(1) vmm_start()
{
   vm_set_entry();
   vmm_vmc_start();
}

void vmm_init()
{
   info->vm.cpu.dflt_excp = VM_RMODE_EXCP_BITMAP;
   vmm_vmc_init();
   vmm_dev_init();
}
