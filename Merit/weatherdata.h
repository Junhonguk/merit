#ifndef _WEATHERDATA_H_
#define _WEATHERDATA_H_

class WeatherData
{
public:
    WeatherData():
        Temperature( 0.0 ),
        DirectSolar( 0.0 ),
        DiffuseSolar( 0.0 ),
		WindSpeed( 0.0 ),
		WindDirection (0.0),
		RelativeHumidity (0.0)
    {
    }

    WeatherData( double t, double Qdr, double Qdf, double Vw, double Dw, double rh ):
        Temperature( t ),
        DirectSolar( Qdr ),
        DiffuseSolar( Qdf ),
        WindSpeed( Vw ),
        WindDirection( Dw ),
        RelativeHumidity( rh )
    {
    }
	double Temperature;
	double DirectSolar;
	double DiffuseSolar;
	double WindSpeed;
	double WindDirection;
	double RelativeHumidity;
};

#endif
