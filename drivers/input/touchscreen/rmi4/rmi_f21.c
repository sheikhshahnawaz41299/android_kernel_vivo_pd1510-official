/*
 * Copyright (c) 2012-2012 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#define FUNCTION_DATA rmi_fn_21_data
#define FNUM 21


#include <linux/kernel.h>
#include <linux/rmi.h>
#include <linux/slab.h>
#include <linux/vivo_touchscreen_common.h>
#include "rmi_driver.h"

union f21_2df_query {
	struct {
		/* Query 0 */
		u8 max_force_sensor_count:3;
		u8 f21_2df_query0_b3__6:4;
		u8 has_high_resolution:1;
	} __attribute__((__packed__));
	struct {
		u8 regs[1];
		u16 address;
	} __attribute__((__packed__));
};

union f21_2df_control_0__3 {
	struct {
		/* Control 0 */
		u8 reporting_mode:3;
		u8 no_rezero:1;
		u8 ctl0_reserved:4;

		/* Control 1 */
		u8 force_click_threshold:8;

		/* Control 2 */
		u8 int_en_force_0:1;
		u8 int_en_force_1:1;
		u8 int_en_force_2:1;
		u8 int_en_force_3:1;
		u8 int_en_force_4:1;
		u8 int_en_force_5:1;
		u8 int_en_force_6:1;
		u8 int_en_click:1;

		/* Control 3 */
		u8 force_interrupt_threshold:8;
	} __attribute__((__packed__));
	struct {
		u8 regs[4];
		u16 address;
	} __attribute__((__packed__));
};

struct f21_2df_control_4n {
	/*Control 4.* */
	u8 one_newton_linear_calibration:7;
	u8 use_cfg_cal:1;
} __attribute__((__packed__));

struct f21_2df_control_4 {
		struct f21_2df_control_4n *regs;
		u16 address;
		u8 length;
} __attribute__((__packed__));

struct f21_2df_control_5n {
	/* Control 5 */
	u8 one_newton_nonlinear_calibration;
};

struct f21_2df_control_5 {
		struct f21_2df_control_5n *regs;
		u16 address;
		u8 length;
};

struct f21_2df_control_6n {
	/*Control 6.* */
	u8 x_location;
};

struct f21_2df_control_6 {
		struct f21_2df_control_6n *regs;
		u16 address;
		u8 length;
};

struct f21_2df_control_7n {
	/*Control 7.* */
	u8 y_location;
};

struct f21_2df_control_7 {
		struct f21_2df_control_7n *regs;
		u16 address;
		u8 length;
};

struct f21_2df_control_8n {
	/*Control 8.* */
	u8 transmitter_force_sensor;
};

struct f21_2df_control_8 {
		struct f21_2df_control_8n *regs;
		u16 address;
		u8 length;
};

struct f21_2df_control_9n {
	/*Control 9.* */
	u8 receiver_force_sensor;
};

struct f21_2df_control_9 {
		struct f21_2df_control_9n *regs;
		u16 address;
		u8 length;
};


#define RMI_F21_NUM_CTRL_REGS 8
struct f21_2df_control {
	/* Control 0-3 */
	union f21_2df_control_0__3 *reg_0__3;

	/* Control 4 */
	struct f21_2df_control_4 *reg_4;

	/* Control 5 */
	struct f21_2df_control_5 *reg_5;

	/* Control 6 */
	struct f21_2df_control_6 *reg_6;

	/* Control 7 */
	struct f21_2df_control_7 *reg_7;

	/* Control 8 */
	struct f21_2df_control_8 *reg_8;

	/* Control 9 */
	struct f21_2df_control_9 *reg_9;
};

union f21_2df_data_2 {
	struct {
		/* Data 2 */
		u8 force_click:1;
		u8 f21_2df_control0_b2__7:7;
	} __attribute__((__packed__));
	struct {
		u8 regs[1];
		u16 address;
	} __attribute__((__packed__));
};

struct f21_2df_data {
	/* Data 0 */
	struct {
		u8 *force_hi_lo;
		u16 address;
	} reg_0__1;

	/* Data 2 */
	union f21_2df_data_2 *reg_2;
};

#define F21_REZERO_CMD 0x01

/* data specific to fn $21 that needs to be kept around */
struct rmi_fn_21_data {
	union f21_2df_query query;
	struct f21_2df_control control;
	struct f21_2df_data data;

	struct mutex control_mutex;
	struct mutex data_mutex;
};

static int rmi_f21_alloc_memory(struct rmi_function_container *fc);

static void rmi_f21_free_memory(struct rmi_function_container *fc);

static int rmi_f21_initialize(struct rmi_function_container *fc);

static int rmi_f21_config(struct rmi_function_container *fc);

static int rmi_f21_create_sysfs(struct rmi_function_container *fc);

/* Sysfs files */

/* Query sysfs files */


show_union_struct_prototype(max_force_sensor_count)
show_union_struct_prototype(has_high_resolution)

static struct attribute *attrs[] = {
	attrify(max_force_sensor_count),
	attrify(has_high_resolution),
	NULL
};
static struct attribute_group attrs_query = GROUP(attrs);
/* Control sysfs files */

show_store_union_struct_prototype(reporting_mode)
show_store_union_struct_prototype(no_rezero)
show_store_union_struct_prototype(force_click_threshold)
show_store_union_struct_prototype(int_en_force_0)
show_store_union_struct_prototype(int_en_force_1)
show_store_union_struct_prototype(int_en_force_2)
show_store_union_struct_prototype(int_en_force_3)
show_store_union_struct_prototype(int_en_force_4)
show_store_union_struct_prototype(int_en_force_5)
show_store_union_struct_prototype(int_en_force_6)
show_store_union_struct_prototype(int_en_click)
show_store_union_struct_prototype(force_interrupt_threshold)

show_store_union_struct_prototype(use_cfg_cal)
show_store_union_struct_prototype(one_newton_linear_calibration)
show_store_union_struct_prototype(one_newton_nonlinear_calibration)
show_store_union_struct_prototype(x_location)
show_store_union_struct_prototype(y_location)
show_store_union_struct_prototype(transmitter_force_sensor)
show_store_union_struct_prototype(receiver_force_sensor)


static struct attribute *attrs2[] = {
	attrify(reporting_mode),
	attrify(no_rezero),
	attrify(force_click_threshold),
	attrify(int_en_click),
	attrify(force_interrupt_threshold),
	attrify(use_cfg_cal),
	attrify(one_newton_linear_calibration),
	attrify(one_newton_nonlinear_calibration),
	attrify(x_location),
	attrify(y_location),
	attrify(transmitter_force_sensor),
	attrify(receiver_force_sensor),
	NULL
};
static struct attribute_group attrs_control = GROUP(attrs2);

/* Data sysfs files */
show_union_struct_prototype(force)
show_union_struct_prototype(force_click)

static struct attribute *attrs3[] = {
	attrify(force),
	attrify(force_click),
	NULL
};
static struct attribute_group attrs_data = GROUP(attrs3);

/* Command sysfs files */

static ssize_t rmi_fn_21_rezero_store(struct device *dev,
					struct device_attribute *attr,
					const char *buf, size_t count);
DEVICE_ATTR(rezero, RMI_WO_ATTR,
		rmi_show_error,
		rmi_fn_21_rezero_store);

static struct attribute *attrs4[] = {
	attrify(rezero),
	NULL
};
static struct attribute_group attrs_command = GROUP(attrs4);

static int rmi_f21_init(struct rmi_function_container *fc)
{
	int retval = 0;

	VIVO_TS_LOG_INF("[%s]:Intializing F21.\n", __func__);

	retval = rmi_f21_alloc_memory(fc);
	if (retval < 0)
		goto error_exit;

	retval = rmi_f21_initialize(fc);
	if (retval < 0)
		goto error_exit;

	retval = rmi_f21_create_sysfs(fc);
	if (retval < 0)
		goto error_exit;

	return retval;

error_exit:
	rmi_f21_free_memory(fc);

	return retval;
}

static int rmi_f21_alloc_memory(struct rmi_function_container *fc)
{
	struct rmi_fn_21_data *f21;

	f21 = kzalloc(sizeof(struct rmi_fn_21_data), GFP_KERNEL);
	if (!f21) {
		VIVO_TS_LOG_ERR("[%s]:Failed to allocate rmi_fn_21_data.\n", __func__);
		return -ENOMEM;
	}
	fc->data = f21;

	return 0;
}


static void rmi_f21_free_memory(struct rmi_function_container *fc)
{
	struct rmi_fn_21_data *f21 = fc->data;
	u8 int_num = f21->query.max_force_sensor_count;
	sysfs_remove_group(&fc->dev.kobj, &attrs_query);
	sysfs_remove_group(&fc->dev.kobj, &attrs_control);
	switch (int_num) {
	case 7:
		sysfs_remove_file(&fc->dev.kobj, attrify(int_en_force_6));
	case 6:
		sysfs_remove_file(&fc->dev.kobj, attrify(int_en_force_5));
	case 5:
		sysfs_remove_file(&fc->dev.kobj, attrify(int_en_force_4));
	case 4:
		sysfs_remove_file(&fc->dev.kobj, attrify(int_en_force_3));
	case 3:
		sysfs_remove_file(&fc->dev.kobj, attrify(int_en_force_2));
	case 2:
		sysfs_remove_file(&fc->dev.kobj, attrify(int_en_force_1));
	case 1:
		sysfs_remove_file(&fc->dev.kobj, attrify(int_en_force_0));
	default:
		break;
	}
	sysfs_remove_group(&fc->dev.kobj, &attrs_data);
	sysfs_remove_group(&fc->dev.kobj, &attrs_command);
	if (f21) {
		kfree(f21->control.reg_0__3);
		kfree(f21->control.reg_4->regs);
		kfree(f21->control.reg_4);
		kfree(f21->control.reg_5->regs);
		kfree(f21->control.reg_5);
		kfree(f21->control.reg_6->regs);
		kfree(f21->control.reg_6);
		kfree(f21->control.reg_7->regs);
		kfree(f21->control.reg_7);
		kfree(f21->control.reg_8->regs);
		kfree(f21->control.reg_8);
		kfree(f21);
		fc->data = NULL;
	}
}


static int rmi_f21_initialize(struct rmi_function_container *fc)
{
	struct rmi_fn_21_data *instance_data = fc->data;
	int retval = 0;
	u16 next_loc;

	/* Read F21 Query Data */
	instance_data->query.address = fc->fd.query_base_addr;
	retval = rmi_read_block(fc->rmi_dev, instance_data->query.address,
		(u8 *)&instance_data->query, sizeof(instance_data->query.regs));
	if (retval < 0) {
		VIVO_TS_LOG_ERR("[%s]:Could not read query registers from 0x%04x\n", __func__, instance_data->query.address);
		return retval;
	}

	/* Initialize Control Data */
	next_loc = fc->fd.control_base_addr;

	instance_data->control.reg_0__3 =
			kzalloc(sizeof(union f21_2df_control_0__3), GFP_KERNEL);
	if (!instance_data->control.reg_0__3) {
		VIVO_TS_LOG_ERR("[%s]:Failed to allocate control registers.", __func__);
		return -ENOMEM;
	}
	instance_data->control.reg_0__3->address = next_loc;
	next_loc += sizeof(instance_data->control.reg_0__3->regs);

	instance_data->control.reg_4 =
			kzalloc(sizeof(struct f21_2df_control_4), GFP_KERNEL);
	if (!instance_data->control.reg_4) {
		VIVO_TS_LOG_ERR("[%s]:Failed to allocate control register.", __func__);
		return -ENOMEM;
	}
	instance_data->control.reg_4->length = instance_data->query.max_force_sensor_count;
	instance_data->control.reg_4->regs =
			kzalloc(sizeof(struct f21_2df_control_4n) * instance_data->control.reg_4->length, GFP_KERNEL);
	if (!instance_data->control.reg_4->regs) {
		VIVO_TS_LOG_ERR("[%s]:Failed to allocate control registers.", __func__);
		return -ENOMEM;
	}
	instance_data->control.reg_4->address = next_loc;
	next_loc += instance_data->control.reg_4->length;

	instance_data->control.reg_5 =
			kzalloc(sizeof(struct f21_2df_control_5), GFP_KERNEL);
	if (!instance_data->control.reg_5) {
		VIVO_TS_LOG_ERR("[%s]:Failed to allocate control registers.", __func__);
		return -ENOMEM;
	}
	instance_data->control.reg_5->length = instance_data->query.max_force_sensor_count;
	instance_data->control.reg_5->regs =
			kzalloc(sizeof(struct f21_2df_control_5n) * instance_data->control.reg_5->length, GFP_KERNEL);
	if (!instance_data->control.reg_5->regs) {
		VIVO_TS_LOG_ERR("[%s]:Failed to allocate control registers.", __func__);
		return -ENOMEM;
	}
	instance_data->control.reg_5->address = next_loc;
	next_loc += instance_data->control.reg_5->length;

	instance_data->control.reg_6 =
			kzalloc(sizeof(struct f21_2df_control_6), GFP_KERNEL);
	if (!instance_data->control.reg_6) {
		VIVO_TS_LOG_ERR("[%s]:Failed to allocate control registers.", __func__);
		return -ENOMEM;
	}
	instance_data->control.reg_6->length = instance_data->query.max_force_sensor_count;
	instance_data->control.reg_6->regs =
			kzalloc(sizeof(struct f21_2df_control_6n) * instance_data->control.reg_6->length, GFP_KERNEL);
	if (!instance_data->control.reg_6->regs) {
		VIVO_TS_LOG_ERR("[%s]:Failed to allocate control registers.", __func__);
		return -ENOMEM;
	}
	instance_data->control.reg_6->address = next_loc;
	next_loc += instance_data->control.reg_6->length;

	instance_data->control.reg_7 =
			kzalloc(sizeof(struct f21_2df_control_7), GFP_KERNEL);
	if (!instance_data->control.reg_7) {
		VIVO_TS_LOG_ERR("[%s]:Failed to allocate control registers.", __func__);
		return -ENOMEM;
	}
	instance_data->control.reg_7->length = instance_data->query.max_force_sensor_count;
	instance_data->control.reg_7->regs =
			kzalloc(sizeof(struct f21_2df_control_7n) * instance_data->control.reg_7->length, GFP_KERNEL);
	if (!instance_data->control.reg_7->regs) {
		VIVO_TS_LOG_ERR("[%s]:Failed to allocate control registers.", __func__);
		return -ENOMEM;
	}
	instance_data->control.reg_7->address = next_loc;
	next_loc += instance_data->control.reg_7->length;

	instance_data->control.reg_8 =
			kzalloc(sizeof(struct f21_2df_control_8), GFP_KERNEL);
	if (!instance_data->control.reg_8) {
		VIVO_TS_LOG_ERR("[%s]:Failed to allocate control registers.", __func__);
		return -ENOMEM;
	}
	instance_data->control.reg_8->length = instance_data->query.max_force_sensor_count;
	instance_data->control.reg_8->regs =
			kzalloc(sizeof(struct f21_2df_control_8n) * instance_data->control.reg_8->length, GFP_KERNEL);
	if (!instance_data->control.reg_8->regs) {
		VIVO_TS_LOG_ERR("[%s]:Failed to allocate control registers.", __func__);
		return -ENOMEM;
	}
	instance_data->control.reg_8->address = next_loc;
	next_loc += instance_data->control.reg_8->length;

	instance_data->control.reg_9 =
			kzalloc(sizeof(struct f21_2df_control_9), GFP_KERNEL);
	if (!instance_data->control.reg_9) {
		VIVO_TS_LOG_ERR("[%s]:Failed to allocate control registers.", __func__);
		return -ENOMEM;
	}
	instance_data->control.reg_9->length = instance_data->query.max_force_sensor_count;
	instance_data->control.reg_9->regs =
			kzalloc(sizeof(struct f21_2df_control_9n)
			* instance_data->control.reg_9->length, GFP_KERNEL);
	if (!instance_data->control.reg_9->regs) {
		VIVO_TS_LOG_ERR("[%s]:Failed to allocate control registers.", __func__);
		return -ENOMEM;
	}
	instance_data->control.reg_9->address = next_loc;


	/* initialize data registers */
	next_loc = fc->fd.data_base_addr;

	instance_data->data.reg_0__1.force_hi_lo = kzalloc(
		2 * instance_data->query.max_force_sensor_count * sizeof(u8),
								GFP_KERNEL);
	if (!instance_data->data.reg_0__1.force_hi_lo) {
		VIVO_TS_LOG_ERR("[%s]:Failed to allocate data registers.", __func__);
		return -ENOMEM;
	}
	instance_data->data.reg_0__1.address = next_loc;
	next_loc += 2 * instance_data->query.max_force_sensor_count;

	instance_data->data.reg_2 =
			kzalloc(sizeof(union f21_2df_data_2), GFP_KERNEL);
	if (!instance_data->data.reg_2) {
		VIVO_TS_LOG_ERR("[%s]:Failed to allocate data registers.", __func__);
		return -ENOMEM;
	}
	instance_data->control.reg_0__3->address = next_loc;

	mutex_init(&instance_data->control_mutex);
	return 0;
}


static int rmi_f21_create_sysfs(struct rmi_function_container *fc)
{
	struct rmi_fn_21_data *instance_data = fc->data;
	u8 int_num = instance_data->query.max_force_sensor_count;
	if (int_num > 7)
		int_num = 7;
	VIVO_TS_LOG_DBG("[%s]:Creating sysfs files.", __func__);

	/* Set up sysfs device attributes. */
	if (sysfs_create_group(&fc->dev.kobj, &attrs_query) < 0) {
		VIVO_TS_LOG_ERR("[%s]:Failed to create query sysfs files.", __func__);
		return -ENODEV;
	}
	if (sysfs_create_group(&fc->dev.kobj, &attrs_control) < 0) {
		VIVO_TS_LOG_ERR("[%s]:Failed to create control sysfs files.", __func__);
		return -ENODEV;
	}
	switch (int_num) {
	case 7:
		if (sysfs_create_file(&fc->dev.kobj, attrify(int_en_force_6)) < 0) {
			VIVO_TS_LOG_ERR("[%s]:Failed to create control sysfs files.", __func__);
			return -ENODEV;
		}
	case 6:
		if (sysfs_create_file(&fc->dev.kobj, attrify(int_en_force_5)) < 0) {
			VIVO_TS_LOG_ERR("[%s]:Failed to create control sysfs files.", __func__);
			return -ENODEV;
		}
	case 5:
		if (sysfs_create_file(&fc->dev.kobj, attrify(int_en_force_4)) < 0) {
			VIVO_TS_LOG_ERR("[%s]:Failed to create control sysfs files.", __func__);
			return -ENODEV;
		}
	case 4:
		if (sysfs_create_file(&fc->dev.kobj, attrify(int_en_force_3)) < 0) {
			VIVO_TS_LOG_ERR("[%s]:Failed to create control sysfs files.", __func__);
			return -ENODEV;
		}
	case 3:
		if (sysfs_create_file(&fc->dev.kobj, attrify(int_en_force_2)) < 0) {
			VIVO_TS_LOG_ERR("[%s]: Failed to create control sysfs files.", __func__);
			return -ENODEV;
		}
	case 2:
		if (sysfs_create_file(&fc->dev.kobj, attrify(int_en_force_1)) < 0) {
			VIVO_TS_LOG_ERR("[%s]:Failed to create control sysfs files.", __func__);
			return -ENODEV;
		}
	case 1:
		if (sysfs_create_file(&fc->dev.kobj, attrify(int_en_force_0)) < 0) {
			VIVO_TS_LOG_ERR("[%s]:Failed to create control sysfs files.", __func__);
			return -ENODEV;
		}
	default:
		break;
	}
	if (sysfs_create_group(&fc->dev.kobj, &attrs_data) < 0) {
		VIVO_TS_LOG_ERR("[%s]:Failed to create data sysfs files.", __func__);
		return -ENODEV;
	}
	if (sysfs_create_group(&fc->dev.kobj, &attrs_command) < 0) {
		VIVO_TS_LOG_ERR("[%s]:Failed to create command sysfs files.", __func__);
		return -ENODEV;
	}
	return 0;
}


static int rmi_f21_config(struct rmi_function_container *fc)
{
	struct rmi_fn_21_data *data = fc->data;
/* repeated register functions */

	/* Write Control Register values back to device */
	rmi_write_block(fc->rmi_dev, data->control.reg_0__3->address,
				(u8 *)data->control.reg_0__3,
				sizeof(data->control.reg_0__3->regs));

	rmi_write_block(fc->rmi_dev, data->control.reg_4->address,
			(u8 *) data->control.reg_4->regs,
			data->control.reg_4->length);

	rmi_write_block(fc->rmi_dev, data->control.reg_5->address,
			(u8 *) data->control.reg_5->regs,
			data->control.reg_5->length);

	rmi_write_block(fc->rmi_dev, data->control.reg_6->address,
			(u8*) data->control.reg_6->regs,
			data->control.reg_6->length);

	rmi_write_block(fc->rmi_dev, data->control.reg_7->address,
			(u8 *) data->control.reg_7->regs,
			data->control.reg_7->length);

	rmi_write_block(fc->rmi_dev, data->control.reg_8->address,
			(u8 *) data->control.reg_8->regs,
			data->control.reg_8->length);

	rmi_write_block(fc->rmi_dev, data->control.reg_9->address,
			(u8 *) data->control.reg_9->regs,
			data->control.reg_9->length);

	return 0;
}

static void rmi_f21_remove(struct rmi_function_container *fc)
{

	VIVO_TS_LOG_INF("[%s]:Removing F21.", __func__);
	rmi_f21_free_memory(fc);
}

/* sysfs functions */
/* Query */
simple_show_union_struct_unsigned(query, max_force_sensor_count)
simple_show_union_struct_unsigned(query, has_high_resolution)

/* Control */
show_store_union_struct_unsigned(control, reg_0__3, reporting_mode)
show_store_union_struct_unsigned(control, reg_0__3, no_rezero)

show_store_union_struct_unsigned(control, reg_0__3, force_click_threshold)

show_store_union_struct_unsigned(control, reg_0__3, int_en_force_0)
show_store_union_struct_unsigned(control, reg_0__3, int_en_force_1)
show_store_union_struct_unsigned(control, reg_0__3, int_en_force_2)
show_store_union_struct_unsigned(control, reg_0__3, int_en_force_3)
show_store_union_struct_unsigned(control, reg_0__3, int_en_force_4)
show_store_union_struct_unsigned(control, reg_0__3, int_en_force_5)
show_store_union_struct_unsigned(control, reg_0__3, int_en_force_6)
show_store_union_struct_unsigned(control, reg_0__3, int_en_click)
show_store_union_struct_unsigned(control, reg_0__3, force_interrupt_threshold)


/* repeated register functions */
show_store_repeated_union_struct_unsigned(control, reg_4, use_cfg_cal)
show_store_repeated_union_struct_unsigned(control, reg_4, one_newton_linear_calibration)
show_store_repeated_union_struct_unsigned(control, reg_5, one_newton_nonlinear_calibration)
show_store_repeated_union_struct_unsigned(control, reg_6, x_location)
show_store_repeated_union_struct_unsigned(control, reg_7, y_location)
show_store_repeated_union_struct_unsigned(control, reg_8, transmitter_force_sensor)
show_store_repeated_union_struct_unsigned(control, reg_9, receiver_force_sensor)

/* Data */
static ssize_t rmi_fn_21_force_show(struct device *dev,
					struct device_attribute *attr,
					char *buf) {
	struct rmi_function_container *fc;
	struct FUNCTION_DATA *data;
	int reg_length;
	int result, size = 0;
	char *temp;
	int i;

	fc = to_rmi_function_container(dev);
	data = fc->data;

	/* Read current regtype values */
	reg_length = data->query.max_force_sensor_count;
	result = rmi_read_block(fc->rmi_dev, data->data.reg_0__1.address,
			data->data.reg_0__1.force_hi_lo,
			2 * reg_length * sizeof(u8));

	if (result < 0) {
		VIVO_TS_LOG_ERR("[%s]: Could not read regtype at 0x%x\nData may be outdated.", __func__,
					data->data.reg_0__1.address);
	}
	temp = buf;
	for (i = 0; i < reg_length; i++) {
		result = snprintf(temp, PAGE_SIZE - size, "%d ",
			data->data.reg_0__1.force_hi_lo[i] * (2 << 3)
			+ data->data.reg_0__1.force_hi_lo[i + reg_length]);
		if (result < 0) {
			VIVO_TS_LOG_ERR("[%s]: Could not write output.", __func__);
			return result;
		}
		size += result;
		temp += result;
	}
	result = snprintf(temp, PAGE_SIZE - size, "\n");
	if (result < 0) {
			VIVO_TS_LOG_ERR("[%s]: Could not write output.", __func__);
			return result;
	}
	return size + result;
}

show_union_struct_unsigned(data, reg_2, force_click)

/* Command */

static ssize_t rmi_fn_21_rezero_store(struct device *dev,
				   struct device_attribute *attr,
				   const char *buf, size_t count) {
	unsigned long val;
	int error, result;
	struct rmi_function_container *fc;
	struct rmi_fn_21_data *instance_data;
	struct rmi_driver *driver;
	u8 command;

	fc = to_rmi_function_container(dev);
	instance_data = fc->data;
	driver = fc->rmi_dev->driver;

	/* need to convert the string data to an actual value */
	error = strict_strtoul(buf, 10, &val);
	if (error)
		return error;
	/* Do nothing if not set to 1. This prevents accidental commands. */
	if (val != 1)
		return count;

	command = (unsigned char)F21_REZERO_CMD;

	/* Write the command to the command register */
	result = rmi_write_block(fc->rmi_dev, fc->fd.command_base_addr,
						&command, 1);
	if (result < 0) {
		VIVO_TS_LOG_ERR("[%s]: Could not write command to 0x%x\n",
				__func__, fc->fd.command_base_addr);
		return result;
	}
	return count;
}

static struct rmi_function_handler function_handler = {
	.func = 0x21,
	.init = rmi_f21_init,
	.config = rmi_f21_config,
	.reset = NULL,
	.attention = NULL,
	.remove = rmi_f21_remove
};

static int __init rmi_f21_module_init(void)
{
	int error;

	error = rmi_register_function_driver(&function_handler);
	if (error < 0) {
		VIVO_TS_LOG_ERR("[%s]: register failed!\n", __func__);
		return error;
	}
	return 0;
}

static void rmi_f21_module_exit(void)
{
	rmi_unregister_function_driver(&function_handler);
}

module_init(rmi_f21_module_init);
module_exit(rmi_f21_module_exit);

MODULE_AUTHOR("Daniel Rosenberg <daniel.rosenberg@synaptics.com>");
MODULE_DESCRIPTION("RMI F21 module");
MODULE_LICENSE("GPL");
MODULE_VERSION(RMI_DRIVER_VERSION);
