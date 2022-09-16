#pragma once
#include "../src/Drawing/DrawShapes.h"



class Bernoulli;

template<typename T1, typename T2>
std::string _Format(T1 f, T2 n);

namespace Shapes
{
	//We inherit our DrawShipes class into our specialized class for our bernoulli simulation
	class Pipes : public DrawShapes
	{
	private:
		float Inlet_Diameter;
		float Outlet_Diameter;
		float height;
		float Shape_Length;
		ImU32 PipeColor;
		ImVec2 Position;
		float length;
		float diameter;
	public:
		//We first initalize the DrawShapes class then we pass the parameters of our shapes
		Pipes() : DrawShapes()
		{
			diameter = 10;
			Position.x = 100; Position.y = 450; length = 150;
			height = 150;
			PipeColor = ImColor(106, 121, 92, 200);
		}
		Pipes(float Height, float Diameter) : DrawShapes()
		{
			diameter = Diameter;
			Position.x = 250; Position.y = 390; length = 150;
			height = Height;
			PipeColor = ImColor(106, 121, 92, 200);
		}
		//We pass our ImDrawList so it can be used to draw the shapes
		//outlet_multiplier is variable that scales our outlet diameter in terms of diameter multiplier
		//bern is our object which will be used to retrieve the data of our current conditions and draw it to the user
		void Draw(ImDrawList* draw_list, float &outlet_multiplier, Bernoulli bern);
		ImU32 returnColor() { return col32; }
		ImU32 returnColorText() { return TextColor; }
		float& ReturnInlet() {return Inlet_Diameter;}
		float& ReturnOutlet() {return Outlet_Diameter;}
		float& ReturnHeight() {return height;}

	};
}


//our Bernoulli class which will store our process conditions and also the Bernoulli_Simulation function
class Bernoulli
{
private:
	Shapes::Pipes _pipes; //Shapes object
	float inlet_diameter_ = 20.0f; //diameter in cm
	float outlet_diameter_ = 40.0f; //diameter in cm
	float height_ = 80.0f; //height in cm
	float pressure1_ = 110.0f; //pressure in kpa
	float pressure2_ = 200.0f; //pressure in kpa
	float velocity1_ = 5.0f; //Velocity in m/s
	float velocity2_ = 0.0f; //Velocity in m/s
	float fluid_density_ = 997.77f; //in density of water at room temperature 22c
	float Gravity = 9.81f;

	float PressureChange = 0;
	float VolumeFlowRateIn = 0;
	float MassFlowRateIn = 0;
	float VolumeFlowRateOut = 0;
	float MassFlowRateOut = 0;


	bool KineticProfile = false;

	float KineticEnergy1 = 0;
	float KineticEnergy2 = 0;
	float PotentialEnergy = 0;
public:
	Bernoulli() : _pipes()
	{
		CalculateVelocity();
		CalculatePressure();
		CalculateFlowRateIn();
		CalculateMassFlowRateIn();
		CalculateFlowRateOut();
		CalculateMassFlowRateOut();
		CalculateKineticEnergy1();
		CalculateKineticEnergy2();
		CalculatePotentialEnergy();
	}

	void Draw(ImDrawList* draw_list); //will be used to draw our shapes
	void setPipeDraw(float height, float diameter); //function that sets our _pipes paramters
	static void Bernoulli_Simulation(bool* p_open); //Static function that will bind to our Chemotar program as a function pointer

	float* returnOutlet() {return &outlet_diameter_;}
	float* returnHeight() {return &height_;}
	float* return_pressure1() {return &pressure1_;}
	float* return_pressure2() {return &pressure2_;}
	float* return_velocity1() { return &velocity1_;}


	//return kinetic profile
	bool* returnKineticProfile() {return &KineticProfile;}

	void CalculateVelocity() { velocity2_ = (pow(inlet_diameter_*0.5, 2)*velocity1_)/(pow(outlet_diameter_*0.5, 2)); }
	void CalculatePressure();
	void CalculateFlowRateIn() {VolumeFlowRateIn = pow(inlet_diameter_ * 0.5/100, 2)*3.14159*velocity1_;}
	void CalculateMassFlowRateIn() {MassFlowRateIn = VolumeFlowRateIn*fluid_density_;}

	//Calculates the Kinetic Energies
	void CalculateKineticEnergy1() {KineticEnergy1 = 0.5*MassFlowRateIn*pow(velocity1_, 2)/(VolumeFlowRateIn*1000);}
	void CalculateKineticEnergy2() {KineticEnergy2 = 0.5*MassFlowRateOut*pow(velocity2_, 2)/(VolumeFlowRateOut*1000);}

	//Calculates the Potential Energies
	void CalculatePotentialEnergy() {PotentialEnergy = MassFlowRateIn*Gravity*height_/(VolumeFlowRateIn * 1000 * 100);}

	//Kinetic Information
	std::string returnPressureEnergy1String() {return "Pressure Energy: " + _Format(pressure1_, 4) + " kj/m^3";}
	std::string returnPressureEnergy2String() {return "Pressure Energy: " + _Format(pressure2_, 4) + " kj/m^3";}
	std::string returnKineticEnergy1String() {return "Kinetic Energy: " + _Format(KineticEnergy1, 4) + " kj/m^3";}
	std::string returnKineticEnergy2String() {return "Kinetic Energy: " + _Format(KineticEnergy2, 4) + " kj/m^3";}
	std::string returnPotentialEnergy() {return "Potential Energy Change: " + _Format(PotentialEnergy, 4) + " kj/m^3";}

	void CalculateFlowRateOut() {VolumeFlowRateOut = pow(outlet_diameter_ * 0.5/100, 2)*3.14159*velocity2_; }
	void CalculateMassFlowRateOut() {MassFlowRateOut = VolumeFlowRateOut* fluid_density_;}
	std::string returnHeightString() const {return _Format(height_/100.0f, 3) + " m";}
	std::string returnInletDiameterString() const {return "Diameter: " + _Format(inlet_diameter_, 4) + " cm";}
	std::string returnOutletDiameterString() const {return "Diameter: " + _Format(outlet_diameter_, 4) + " cm";}
	std::string returnPressure1String() const {return "Pressure: " + _Format(pressure1_, 4) + " kpa";}
	std::string returnPressure2String() const {return "Pressure: " + _Format(pressure2_, 4) + " kpa";}
	std::string returnVelocity1String() const {return "Velocity: " + _Format(velocity1_, 4) + " m/s";}
	std::string returnVelocity2String() const {return "Velocity: " + _Format(velocity2_, 4) + " m/s";}
	std::string returnFlowRate1String() const {return "Flow Rate: " + _Format(VolumeFlowRateIn, 4) + " m^3/s";}
	std::string returnMassFlowRate1String() const {return "Mass Flow Rate: " + _Format(MassFlowRateIn, 4) + " kg/s";}
	std::string returnFlowRate2String() const {return "Flow Rate: " + _Format(VolumeFlowRateOut, 4) + " m^3/s";}
	std::string returnMassFlowRate2String() const {return "Mass Flow Rate: " + _Format(MassFlowRateOut, 4) + " kg/s";}
};

