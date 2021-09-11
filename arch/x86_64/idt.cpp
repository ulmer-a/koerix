#include <types.h>
#include <context.h>
#include <x86/ports.h>
#include <debug.h>

#define IDT_PRESENT     BIT(7)
#define IDT_USER        (BIT(5)|BIT(6))
#define IDT_SUPV        0
#define IDT_GATE        0b1110
#define IDT_TRAP        0b1111

struct IrqDescriptor
{
    uint16_t  target0;
    uint16_t  target_selector;
    uint8_t   ist;
    uint8_t   flags;
    uint16_t  target1;
    uint32_t  target2;
    uint32_t  pad1;
} _PACKED;

struct IdtSelector
{
    uint16_t  size;
    size_t   addr;
} _PACKED;

static IrqDescriptor g_idt[256];

static void setup_pic()
{
  debug() << "reconfigure PIC\n";

  outb(0x20, 0x11);   // init PIC1
  outb(0xA0, 0x11);   // init PIC2

  outb(0x21, 0x20);   // start interrupts at 0x20 on PIC1
  outb(0xA1, 0x28);   // start interrupts at 0x28 on PIC2

  outb(0x21, 0x04);   // PIC1 -> master
  outb(0xA1, 0x02);   // PIC2 -> slave

  outb(0x21, 0x01);
  outb(0xA1, 0x01);

  outb(0x21, 0x00);   // unmask all interrupts PIC1
  outb(0xA1, 0x00);   // unmask all interrupts PIC2
}

static void install_descriptor(unsigned id, void *handler, uint8_t flags, uint8_t stack)
{
  size_t target_addr = (size_t)handler;

  IrqDescriptor *irqd = g_idt + id;
  irqd->target0 = target_addr & 0xffff;
  irqd->target1 = (target_addr >> 16) & 0xffff;
  irqd->target2 = (target_addr >> 32);
  irqd->target_selector = 0x08;
  irqd->flags = flags;
  irqd->ist = stack;
}

extern "C" {
    // irq handler addresses
    extern char irq0;  extern char irq1;  extern char irq2;  extern char irq3;
    extern char irq4;  extern char irq5;  extern char irq6;  extern char irq7;
    extern char irq8;  extern char irq9;  extern char irq10; extern char irq11;
    extern char irq12; extern char irq13; extern char irq14; extern char irq15;
    extern char irq16; extern char irq17; extern char irq18; extern char irq19;
    extern char irq20; extern char irq21; extern char irq22; extern char irq23;
    extern char irq24; extern char irq25; extern char irq26; extern char irq27;
    extern char irq28; extern char irq29; extern char irq30; extern char irq31;
    extern char irq32; extern char irq33; extern char irq34; extern char irq35;
    extern char irq36; extern char irq37; extern char irq38; extern char irq39;
    extern char irq40; extern char irq41; extern char irq42; extern char irq43;
    extern char irq44; extern char irq45; extern char irq46; extern char irq47;

    extern char irq_syscall;
}

static void setup_idt()
{
    debug() << "Setting up interrupt descriptor table\n";

  install_descriptor(0,  &irq0,  IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(1,  &irq1,  IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(2,  &irq2,  IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(3,  &irq3,  IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(4,  &irq4,  IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(5,  &irq5,  IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(6,  &irq6,  IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(7,  &irq7,  IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(8,  &irq8,  IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(9,  &irq9,  IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(10, &irq10, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(11, &irq11, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(12, &irq12, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(13, &irq13, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(14, &irq14, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(15, &irq15, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(16, &irq16, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(17, &irq17, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(18, &irq18, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(19, &irq19, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(20, &irq20, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(21, &irq21, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(22, &irq22, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(23, &irq23, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(24, &irq24, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(25, &irq25, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(26, &irq26, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(27, &irq27, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(28, &irq28, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(29, &irq29, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(30, &irq30, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(31, &irq31, IDT_PRESENT | IDT_USER | IDT_GATE, 0); // YIELD
  install_descriptor(32, &irq32, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(33, &irq33, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(34, &irq34, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(35, &irq35, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(36, &irq36, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(37, &irq37, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(38, &irq38, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(39, &irq39, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(40, &irq40, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(41, &irq41, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(42, &irq42, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(43, &irq43, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(44, &irq44, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(45, &irq45, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(46, &irq46, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);
  install_descriptor(47, &irq47, IDT_PRESENT | IDT_SUPV | IDT_GATE, 0);

  /* setup a call gate for the system call handler (int $0x80) */
  install_descriptor(0x80, &irq_syscall, IDT_PRESENT | IDT_USER | IDT_GATE, 0);

  // tell the processor where the IDT is located
  IdtSelector idtSelector;
  idtSelector.size = sizeof(IrqDescriptor) * 256 - 1;
  idtSelector.addr = (size_t)g_idt;
  __asm__ volatile ("lidt %0;" : : "g"(idtSelector));
}

extern "C" void x86_irq_init()
{
  setup_pic();
  setup_idt();
}
