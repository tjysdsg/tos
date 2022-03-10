#include "acpi_driver.h"
#include "kpanic.h"
// #include <acpi.h>

#define CHECK_ACPI_STATUS(status)   \
do {                                \
  if (ACPI_FAILURE(status)) {       \
  kpanic("Cannot initialize ACPI"); \
  }                                 \
} while(false)

void init_acpi() {
  // ACPI_STATUS status;

  // status = AcpiInitializeSubsystem();
  // CHECK_ACPI_STATUS(status);

  // status = AcpiInitializeTables(nullptr, 16, true);
  // CHECK_ACPI_STATUS(status);

  // status = AcpiLoadTables();
  // CHECK_ACPI_STATUS(status);

  // status = AcpiEnableSubsystem(ACPI_FULL_INITIALIZATION);
  // CHECK_ACPI_STATUS(status);

  // status = AcpiInitializeObjects(ACPI_FULL_INITIALIZATION);
  // CHECK_ACPI_STATUS(status);
}
