/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _BMC150_ACCEL_H_
#define _BMC150_ACCEL_H_

#include <linux/atomic.h>
#include <linux/iio/iio.h>
#include <linux/mutex.h>
#include <linux/regulator/consumer.h>
#include <linux/workqueue.h>

#include <linux/i2c.h>
#include <linux/spi/spi.h>

struct regmap;
struct i2c_client;
struct bmc150_accel_chip_info;
struct bmc150_accel_interrupt_info;

/*
 * We can often guess better than "UNKNOWN" based on the device IDs
 * but unfortunately this information is not always accurate. There are some
 * devices where ACPI firmware specifies an ID like "BMA250E" when the device
 * actually has a BMA222E. The driver attempts to detect those by reading the
 * chip ID from the registers but this information is not always enough either.
 *
 * Therefore, this enum should be only used when the chip ID detection is not
 * enough and we can be reasonably sure that the device IDs are reliable
 * in practice (e.g. for device tree platforms).
 */
enum bmc150_type {
	BOSCH_UNKNOWN,
	BOSCH_BMC156,
};

struct bmc150_accel_interrupt {
	const struct bmc150_accel_interrupt_info *info;
	atomic_t users;
};

enum bmc150_device_type {
	BMC150,
	BMI323,
};

struct bmc150_accel_trigger {
	struct bmc150_accel_data *data;
	struct iio_trigger *indio_trig;
	int (*setup)(struct bmc150_accel_trigger *t, bool state);
	int intr;
	bool enabled;
};

enum bmc150_accel_interrupt_id {
	BMC150_ACCEL_INT_DATA_READY,
	BMC150_ACCEL_INT_ANY_MOTION,
	BMC150_ACCEL_INT_WATERMARK,
	BMC150_ACCEL_INTERRUPTS,
};

enum bmc150_accel_trigger_id {
	BMC150_ACCEL_TRIGGER_DATA_READY,
	BMC150_ACCEL_TRIGGER_ANY_MOTION,
	BMC150_ACCEL_TRIGGERS,
};

#define BMI323_FLAGS_RESET_FAILED 0x00000001U

struct bmi323_private_data {
	struct regulator_bulk_data regulators[2];
	struct i2c_client* i2c_client;
	struct spi_device* spi_client;
	struct device* dev; // pointer at i2c_client->dev or spi_client->dev
	struct mutex mutex;
	int irq;
	uint32_t flags;
};

struct bmc150_accel_data {
	struct regmap *regmap;
	struct regulator_bulk_data regulators[2];
	struct bmc150_accel_interrupt interrupts[BMC150_ACCEL_INTERRUPTS];
	struct bmc150_accel_trigger triggers[BMC150_ACCEL_TRIGGERS];
	struct mutex mutex;
	u8 fifo_mode, watermark;
	s16 buffer[8];
	/*
	 * Ensure there is sufficient space and correct alignment for
	 * the timestamp if enabled
	 */
	struct {
		__le16 channels[3];
		s64 ts __aligned(8);
	} scan;
	u8 bw_bits;
	u32 slope_dur;
	u32 slope_thres;
	u32 range;
	int ev_enable_state;
	int64_t timestamp, old_timestamp; /* Only used in hw fifo mode. */
	const struct bmc150_accel_chip_info *chip_info;
	enum bmc150_type type;
	struct i2c_client *second_device;
	void (*resume_callback)(struct device *dev);
	struct delayed_work resume_work;
	struct iio_mount_matrix orientation;
	enum bmc150_device_type dev_type;
	struct bmi323_private_data bmi323;
	};

#define BMC150_BMI323_AUTO_SUSPEND_DELAY_MS 2000

#define BMC150_BMI323_CHIP_ID_REG 0x00
#define BMC150_BMI323_SOFT_RESET_REG 0x7E
#define BMC150_BMI323_SOFT_RESET_VAL 0xDEAF
#define BMC150_BMI323_DATA_BASE_REG 0x03
#define BMC150_BMI323_TEMPERATURE_DATA_REG 0x09
#define BMC150_BMI323_ACC_CONF_REG 0x20
#define BMC150_BMI323_GYR_CONF_REG 0x21

// these are bits [0:3] of ACC_CONF.acc_odr, sample rate in Hz for the accel part of the chip
#define BMC150_BMI323_ACCEL_ODR_0_78123_VAL		0x0001
#define BMC150_BMI323_ACCEL_ODR_1_5625_VAL		0x0002
#define BMC150_BMI323_ACCEL_ODR_3_125_VAL		0x0003
#define BMC150_BMI323_ACCEL_ODR_6_25_VAL		0x0004
#define BMC150_BMI323_ACCEL_ODR_12_5_VAL		0x0005
#define BMC150_BMI323_ACCEL_ODR_25_VAL			0x0006
#define BMC150_BMI323_ACCEL_ODR_50_VAL			0x0007
#define BMC150_BMI323_ACCEL_ODR_100_VAL			0x0008
#define BMC150_BMI323_ACCEL_ODR_200_VAL			0x0009
#define BMC150_BMI323_ACCEL_ODR_400_VAL			0x000A
#define BMC150_BMI323_ACCEL_ODR_800_VAL			0x000B
#define BMC150_BMI323_ACCEL_ODR_1600_VAL		0x000C
#define BMC150_BMI323_ACCEL_ODR_3200_VAL		0x000D
#define BMC150_BMI323_ACCEL_ODR_6400_VAL		0x000E

// these are bits [4:6] of ACC_CONF.acc_range, full scale resolution
#define BMC150_BMI323_ACCEL_RANGE_2_VAL		0x0000 // +/-2g, 16.38 LSB/mg
#define BMC150_BMI323_ACCEL_RANGE_4_VAL		0x0001 // +/-4g, 8.19 LSB/mg
#define BMC150_BMI323_ACCEL_RANGE_8_VAL		0x0002 // +/-8g, 4.10 LSB/mg
#define BMC150_BMI323_ACCEL_RANGE_16_VAL	0x0003 // +/-4g, 2.05 LSB/mg

// these are bits [0:3] of GYR_CONF.gyr_odr, sample rate in Hz for the gyro part of the chip
#define BMC150_BMI323_GYRO_ODR_0_78123_VAL		0x0001
#define BMC150_BMI323_GYRO_ODR_1_5625_VAL		0x0002
#define BMC150_BMI323_GYRO_ODR_3_125_VAL		0x0003
#define BMC150_BMI323_GYRO_ODR_6_25_VAL		0x0004
#define BMC150_BMI323_GYRO_ODR_12_5_VAL		0x0005
#define BMC150_BMI323_GYRO_ODR_25_VAL			0x0006
#define BMC150_BMI323_GYRO_ODR_50_VAL			0x0007
#define BMC150_BMI323_GYRO_ODR_100_VAL			0x0008
#define BMC150_BMI323_GYRO_ODR_200_VAL			0x0009
#define BMC150_BMI323_GYRO_ODR_400_VAL			0x000A
#define BMC150_BMI323_GYRO_ODR_800_VAL			0x000B
#define BMC150_BMI323_GYRO_ODR_1600_VAL		0x000C
#define BMC150_BMI323_GYRO_ODR_3200_VAL		0x000D
#define BMC150_BMI323_GYRO_ODR_6400_VAL		0x000E

// these are bits [4:6] of GYR_CONF.gyr_range, full scale resolution
#define BMC150_BMI323_GYRO_RANGE_125_VAL		0x0000 // +/-125°/s, 262.144 LSB/°/s
#define BMC150_BMI323_GYRO_RANGE_250_VAL		0x0001 // +/-250°/s,  131.2 LSB/°/s
#define BMC150_BMI323_GYRO_RANGE_500_VAL		0x0002 // +/-500°/s,  65.6 LSB/°/s
#define BMC150_BMI323_GYRO_RANGE_1000_VAL		0x0003 // +/-1000°/s, 32.8 LSB/°/s
#define BMC150_BMI323_GYRO_RANGE_2000_VAL		0x0004 // +/-2000°/s, 16.4 LSB/°/s

int bmi323_write_u16(struct bmi323_private_data *bmi323, u8 in_reg, u16 in_value);

/**
 * This function performs a read of "good" values from the bmi323 discarding what
 * in the datasheet is described as "dummy data": additional useles bytes.
 *
 * PRE: bmi323 has been partially initialized: i2c_device and spi_devices MUST be set to either
 *      the correct value or NULL
 *
 * NOTE: bmi323->dev can be NULL (not yet initialized) when this function is called
 *			therefore it is not needed and is not used inside the function
 *
 * POST: on success out_value is written with data from the sensor, as it came out so the
 *       content is little-endian even on big endian architectures
 *
 * WARNING: this function does not lock any mutex and synchronization MUST be performed by the caller
 */
int bmi323_read_u16(struct bmi323_private_data *bmi323, u8 in_reg, u16* out_value);

int bmi323_chip_check(struct bmi323_private_data *bmi323);
int bmi323_chip_rst(struct bmi323_private_data *bmi323);

/**
 * This function  MUST be called in probe and is responsible for registering the userspace sysfs.
 *
 * The indio_dev MUST have been allocated but not registered. This function will perform userspace registration.
 *
 * @param indio_dev the industrual io device already allocated but not yet registered
 */
int bmi323_iio_init(struct iio_dev *indio_dev);

void bmi323_iio_deinit(struct iio_dev *indio_dev);

int bmc150_accel_core_probe(struct device *dev, struct regmap *regmap, int irq,
			    enum bmc150_type type, const char *name,
			    bool block_supported);
void bmc150_accel_core_remove(struct device *dev);
extern const struct dev_pm_ops bmc150_accel_pm_ops;
extern const struct regmap_config bmc150_regmap_conf;

#endif  /* _BMC150_ACCEL_H_ */
