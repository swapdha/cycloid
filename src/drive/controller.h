#ifndef DRIVE_CONTROLLER_H_
#define DRIVE_CONTROLLER_H_

#include <Eigen/Dense>
#include <math.h>

#include "drive/config.h"
#include "drive/trajtrack.h"

class DriveController {
 public:
  DriveController();

  void UpdateState(const DriverConfig &config,
      const Eigen::Vector3f &accel,
      const Eigen::Vector3f &gyro,
      uint8_t servo_pos,
      const uint16_t *wheel_encoders, float dt);

  void UpdateLocation(float x, float y, float theta) {
    x_ = x;
    y_ = y;
    theta_ = theta;
  }

  bool GetControl(const DriverConfig &config,
      float throttle_in, float steering_in,
      float *throttle_out, float *steering_out, float dt,
      bool autodrive, int frameno);

  void ResetState();

  int SerializedSize() const { return 0; }
  int Serialize(uint8_t *buf, int buflen) const;

  TrajectoryTracker *GetTracker() { return &track_; }

 private:
  float TargetCurvature(const DriverConfig &config);

  // car state
  float x_, y_, theta_;
  float vf_, vr_;  // front and rear wheel velocity
  float w_;  // yaw rate
  float ierr_v_;  // integration error for velocity
  float ierr_w_;  // integration error for yaw rate
  float delta_;  // current steering angle

  float target_v_, target_w_;  // control targets
  float ye_, sinpsie_, cospsie_, k_;  // relative trajectory target

  TrajectoryTracker track_;
};

#endif  // DRIVE_CONTROLLER_H_
