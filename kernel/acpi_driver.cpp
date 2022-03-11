// #define ENABLE_ACPI

#ifdef ENABLE_ACPI
#ifdef __cplusplus
extern "C" {
#endif
#include <acpi.h>
#ifdef __cplusplus
}
#endif

#endif

#include "acpi_driver.h"
#include "kpanic.h"
#include "memory.h"
#include "isr.h"
#include <stdarg.h>

#define CHECK_ACPI_STATUS(status)   \
do {                                \
  if (ACPI_FAILURE(status)) {       \
  kpanic("Cannot initialize ACPI"); \
  }                                 \
} while(false)

void init_acpi() {
#ifdef ENABLE_ACPI
  ACPI_STATUS status;

  status = AcpiInitializeSubsystem();
  CHECK_ACPI_STATUS(status);

  status = AcpiInitializeTables(nullptr, 16, true);
  CHECK_ACPI_STATUS(status);

  status = AcpiLoadTables();
  CHECK_ACPI_STATUS(status);

  status = AcpiEnableSubsystem(ACPI_FULL_INITIALIZATION);
  CHECK_ACPI_STATUS(status);

  status = AcpiInitializeObjects(ACPI_FULL_INITIALIZATION);
  CHECK_ACPI_STATUS(status);
#endif
}

extern "C" {

#ifdef ENABLE_ACPI
ACPI_STATUS AcpiOsInitialize() {
  return AE_OK;
}

ACPI_STATUS AcpiOsTerminate() {
  kprintf("ACPI shutdown\n");
  return AE_OK;
}

ACPI_PHYSICAL_ADDRESS AcpiOsGetRootPointer() {
  ACPI_PHYSICAL_ADDRESS ret = 0;
  AcpiFindRootPointer(&ret);
  return ret;
}

ACPI_STATUS AcpiOsPredefinedOverride(const ACPI_PREDEFINED_NAMES *PredefinedObject, ACPI_STRING *NewValue) {
  *NewValue = nullptr;
  return AE_OK;
}

ACPI_STATUS AcpiOsTableOverride(ACPI_TABLE_HEADER *ExistingTable, ACPI_TABLE_HEADER **NewTable) {
  *NewTable = nullptr;
  return AE_OK;
}

void *AcpiOsMapMemory(ACPI_PHYSICAL_ADDRESS PhysicalAddress, ACPI_SIZE Length) {
  return (void *) phys2virt(PhysicalAddress, Length);
}

void AcpiOsUnmapMemory(void *LogicalAddress, ACPI_SIZE Length) {
  // TODO: implement AcpiOsUnmapMemory
}

ACPI_STATUS AcpiOsGetPhysicalAddress(void *LogicalAddress, ACPI_PHYSICAL_ADDRESS *PhysicalAddress) {
  *PhysicalAddress = virt2phys((uint32_t) LogicalAddress);
  return AE_OK;
}

void *AcpiOsAllocate(ACPI_SIZE Size) {
  return malloc(Size);
}

void AcpiOsFree(void *Memory) {
  free(Memory);
}

BOOLEAN AcpiOsReadable(void *Memory, ACPI_SIZE Length) {
  // TODO: check if memory page is present and readable
  return true;
}

BOOLEAN AcpiOsWritable(void *Memory, ACPI_SIZE Length) {
  // TODO: check if memory page is present and writable
  return false;
}

// FIXME: need to implement process/thread for the following ACPI functions
ACPI_THREAD_ID AcpiOsGetThreadId() {
  return 1;
}

ACPI_STATUS AcpiOsExecute(ACPI_EXECUTE_TYPE Type, ACPI_OSD_EXEC_CALLBACK Function, void *Context) {
  return AE_NOT_IMPLEMENTED;
}

void AcpiOsSleep(UINT64 Milliseconds) {
  // TODO: sleep(Microseconds * 1000);
}

void AcpiOsStall(UINT32 Microseconds) {
  // TODO: sleep(Microseconds * 1000);
}

// TODO: implement semaphore for the following ACPI functions

ACPI_STATUS AcpiOsCreateSemaphore(UINT32 max, UINT32 initial, ACPI_SEMAPHORE*handle) {
  return AE_OK;
}

ACPI_STATUS AcpiOsDeleteSemaphore(ACPI_SEMAPHORE handle) {
  return AE_OK;
}

ACPI_STATUS AcpiOsWaitSemaphore(ACPI_SEMAPHORE handle, UINT32 units, UINT16 timeout) {
  return AE_OK;
}

ACPI_STATUS AcpiOsSignalSemaphore(ACPI_SEMAPHORE handle, UINT32 units) {
  return AE_OK;
}

ACPI_CPU_FLAGS AcpiOsAcquireLock(ACPI_SPINLOCK lock) {
  //    LOCK_VAL(lock);

  //    return AE_OK;
  return AE_NOT_IMPLEMENTED;
}

void AcpiOsReleaseLock(ACPI_SPINLOCK lock, ACPI_CPU_FLAGS flags) {
  //UNLOCK_VAL(lock);
}

ACPI_STATUS AcpiOsCreateLock(ACPI_SPINLOCK*lock) {
  if (lock == nullptr) {
    return AE_BAD_PARAMETER;
  }

  *lock = new spinlock_t;

  if (*lock == nullptr) {
    return AE_NO_MEMORY;
  }

  return AE_OK;
}

void AcpiOsDeleteLock(ACPI_SPINLOCK lock) {
  free((void *) (lock));
}

struct AcpicaIntHandler {
  ACPI_OSD_HANDLER hand;
  void *context;
};

AcpicaIntHandler acpi_handlers[256];

static void acpi_interrupt_handler(registers_t *regs) {
  acpi_handlers[regs->int_no].hand(acpi_handlers[regs->int_no - IRQ0].context);
}

ACPI_STATUS AcpiOsInstallInterruptHandler(UINT32 InterruptNumber, ACPI_OSD_HANDLER ServiceRoutine, void *Context) {
  acpi_handlers[InterruptNumber].hand = ServiceRoutine;
  acpi_handlers[InterruptNumber].context = Context;
  register_interrupt_handler(InterruptNumber + IRQ0, acpi_interrupt_handler);
  return AE_OK;
}

ACPI_STATUS AcpiOsRemoveInterruptHandler(UINT32 int_no, ACPI_OSD_HANDLER hand) {
  unregister_interrupt_handler(int_no + IRQ0);
  acpi_handlers[int_no].hand = nullptr;
  acpi_handlers[int_no].context = nullptr;
  return AE_OK;
}
#endif

} // extern "C"
