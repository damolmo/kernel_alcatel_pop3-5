//s_add new sensor driver here
//export funtions
UINT32 OV5670_MIPI_RAW_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc);
UINT32 OV5670FF_MIPI_RAW_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc);
UINT32 GC2355_MIPI_RAW_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc);
//! Add Sensor Init function here
//! Note:
//! 1. Add by the resolution from ""large to small"", due to large sensor
//!    will be possible to be main sensor.
//!    This can avoid I2C error during searching sensor.
//! 2. This file should be the same as mediatek\custom\common\hal\imgsensor\src\sensorlist.cpp
ACDK_KD_SENSOR_INIT_FUNCTION_STRUCT kdSensorList[MAX_NUM_OF_SUPPORT_SENSOR+1] =
{
/*IMX*/
#if defined(OV5670_MIPI_RAW)
    {OV5670MIPI_SENSOR_ID, SENSOR_DRVNAME_OV5670_MIPI_RAW, OV5670_MIPI_RAW_SensorInit},
#endif
#if defined(OV5670FF_MIPI_RAW)
    {OV5670FFMIPI_SENSOR_ID, SENSOR_DRVNAME_OV5670FF_MIPI_RAW, OV5670FF_MIPI_RAW_SensorInit},
#endif
#if defined(GC2355_MIPI_RAW)
    {GC2355_SENSOR_ID, SENSOR_DRVNAME_GC2355_MIPI_RAW,GC2355_MIPI_RAW_SensorInit},
#endif

/*  ADD sensor driver before this line */
    {0,{0},NULL}, //end of list
};
//e_add new sensor driver here

