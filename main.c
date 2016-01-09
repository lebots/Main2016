#pragma config(Sensor, dgtl12, piston,         sensorDigitalOut)
#pragma config(Motor,  port1,           liftMotor,     tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           launcherL1,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           launcherL2,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           launcherR1,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           launcherR2,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           flMotor,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           blMotor,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           frMotor,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           brMotor,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          intakeMotor,   tmotorVex393_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// Competition stuff
#pragma platform(VEX)
#pragma competitionControl(Competition)
#pragma autonomousDuration(15)
#pragma userControlDuration(115)
#include "Vex_Competition_Includes.c"

// Include auton files
#include ".\auton\auton1.h"

bool shooterBool = false;
int shootSpeed = 0;
float driveMultiplier = 1;
float shotMultiplier;
long finalSpeed;


void pre_auton() {

}

task autonomous() {
	auton1();
}

task usercontrol()
{
	while (true) { // Btn6U = slow, Btn5U = super slow
		driveMultiplier = vexRT[Btn6U] ? 0.4 : (vexRT[Btn5U] ? 0.25 : 1);

		motor[flMotor] = driveMultiplier * vexRT[Ch3];
		motor[blMotor] = driveMultiplier * vexRT[Ch3];
		motor[frMotor] = driveMultiplier * vexRT[Ch2];
		motor[brMotor] = driveMultiplier * vexRT[Ch2];

		if (vexRT[Btn7LXmtr2]) { // Full court
			shooterBool = true;
			shotMultiplier = 0.54;
		}
		if (vexRT[Btn8RXmtr2]) { // 3/4 court
			shooterBool = true;
			shotMultiplier = 0.475;
		}
		if (vexRT[Btn7UXmtr2]) { // Half court
			shooterBool = true;
			shotMultiplier = 0.4;\
		}
		if (vexRT[Btn8UXmtr2]) { // Quarter court
			shooterBool = true;
			shotMultiplier = 0.35;
		}
		if (vexRT[Btn7DXmtr2] || vexRT[Btn8DXmtr2]) // Stop flywheels
			shooterBool = false;

		// Flywheel rampup logic
		if (shooterBool && shootSpeed < 127)
			shootSpeed = shootSpeed + 2;
		else if (shooterBool && !(shootSpeed < 127))
			shootSpeed = 127;
		else if (!shooterBool && shootSpeed > 0)
			shootSpeed--;
		else
			shootSpeed = 0;

		// Final speed multipliers and stuff
		finalSpeed = shooterBool * shootSpeed * shotMultiplier;

		// Set motor powers
		motor[launcherL1] = finalSpeed;
		motor[launcherL2] = finalSpeed;
		motor[launcherR1] = finalSpeed;
		motor[launcherR2] = finalSpeed;

		// Lift motor and intake motor
		motor[liftMotor] = 127 * (vexRT[Btn5UXmtr2] - vexRT[Btn5DXmtr2]);
		motor[intakeMotor] = 127 * (vexRT[Btn6DXmtr2] - vexRT[Btn6UXmtr2]);

		// Piston to keep balls from flywheels
		if (vexRT[Btn7RXmtr2] || vexRT[Btn8LXmtr2]) {
			SensorValue(piston) = 1;
			} else {
			SensorValue(piston) = 0;
		}

		wait1Msec(50);
	}
}
