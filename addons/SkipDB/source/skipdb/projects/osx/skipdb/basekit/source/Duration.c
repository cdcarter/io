/*#io
docCopyright("Steve Dekorte", 2002)
docLicense("BSD revised")
*/

#define DURATION_C
#include "Duration.h"
#undef DURATION_C
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct
{
	double years;
	double days;
	double hours;
	double minutes;
	double seconds;
} DurationComponents;

Duration *Duration_new(void)
{
	Duration *self = (Duration *)calloc(1, sizeof(Duration));
	return self;
}

Duration *Duration_newWithSeconds_(double s)
{
	Duration *self = Duration_new();
	self->seconds = s;
	return self;
}

void Duration_copy_(Duration *self, Duration *other)
{ 
	memcpy(self, other, sizeof(Duration)); 
}

void Duration_free(Duration *self)
{ 
	free(self); 
}

int Duration_compare(Duration *self, Duration *other)
{ 
	if (self->seconds == other->seconds) 
	{
		return 0;
	}
	
	return self->seconds > other->seconds ? 1 : -1;
}

// components -------------------------------------------------------- 

#define SECONDS_IN_YEAR   (60 * 60 * 24 * 365)
#define SECONDS_IN_DAY    (60 * 60 * 24)
#define SECONDS_IN_HOUR   (60 * 60)
#define SECONDS_IN_MINUTE (60)

DurationComponents Duration_asComponents(Duration *self)
{
	DurationComponents c;
	double t = self->seconds;
	c.years   = (int)(t / SECONDS_IN_YEAR);   t -= (int)(c.years   * SECONDS_IN_YEAR);
	c.days    = (int)(t / SECONDS_IN_DAY);    t -= (int)(c.days    * SECONDS_IN_DAY);
	c.hours   = (int)(t / SECONDS_IN_HOUR);   t -= (int)(c.hours   * SECONDS_IN_HOUR);
	c.minutes = (int)(t / SECONDS_IN_MINUTE); t -= (int)(c.minutes * SECONDS_IN_MINUTE);
	c.seconds = (t);
	return c;
}

void Duration_fromComponents_(Duration *self, DurationComponents c)
{
	double t = c.years * SECONDS_IN_YEAR;
	t += c.days * SECONDS_IN_DAY;
	t += c.hours * SECONDS_IN_HOUR;
	t += c.minutes * SECONDS_IN_MINUTE;
	t += c.seconds;
	self->seconds = t;
}

// years -------------------------------------------------------- 

int Duration_years(Duration *self)
{ 
	return (int)Duration_asComponents(self).years; 
}

void Duration_setYears_(Duration *self, double y)
{ 
	DurationComponents c = Duration_asComponents(self);
	c.years = y;
	Duration_fromComponents_(self, c);
}

// days -------------------------------------------------------- 

int Duration_days(Duration *self)
{ 
	return (int)Duration_asComponents(self).days; 
}

void Duration_setDays_(Duration *self, double d)
{ 
	DurationComponents c = Duration_asComponents(self);
	c.days = d;
	Duration_fromComponents_(self, c);
}

// hours -------------------------------------------------------- 

int Duration_hours(Duration *self)
{ 
	return (int)Duration_asComponents(self).hours; 
}

void Duration_setHours_(Duration *self, double m)
{ 
	DurationComponents c = Duration_asComponents(self);
	c.hours = m;
	Duration_fromComponents_(self, c);
}

// minutes -------------------------------------------------------- 

int Duration_minutes(Duration *self)
{ 
	return (int)Duration_asComponents(self).minutes; 
}

void Duration_setMinutes_(Duration *self, double m)
{ 
	DurationComponents c = Duration_asComponents(self);
	c.minutes = m;
	Duration_fromComponents_(self, c);
}

// seconds -------------------------------------------------------- 

double Duration_seconds(Duration *self)
{ 
	return Duration_asComponents(self).seconds;
}

void Duration_setSeconds_(Duration *self, double s)
{ 
	DurationComponents c = Duration_asComponents(self);
	c.seconds = s;
	Duration_fromComponents_(self, c);
}

// total seconds -------------------------------------------------------- 

double Duration_asSeconds(Duration *self)
{ 
	return self->seconds; 
}

void Duration_fromSeconds_(Duration *self, double s)
{ 
	self->seconds = s; 
}

// strings -------------------------------------------------------- 

ByteArray *Duration_asByteArrayWithFormat_(Duration *self, const char *format)
{
	DurationComponents c = Duration_asComponents(self);
	char s[128];
	ByteArray *ba = ByteArray_newWithCString_(format?format:"%Y years %d days %H:%M:%S");
	
	snprintf(s, 128, "%i", (int)c.years);
	ByteArray_replaceCString_withCString_(ba, "%Y", s);
	
	snprintf(s, 128, "%04i", (int)c.years);
	ByteArray_replaceCString_withCString_(ba, "%y", s);
	
	snprintf(s, 128, "%02i", (int)c.days);
	ByteArray_replaceCString_withCString_(ba, "%d", s);
	
	snprintf(s, 128, "%02i", (int)c.hours);
	ByteArray_replaceCString_withCString_(ba, "%H", s);
	
	snprintf(s, 128, "%02i", (int)c.minutes);
	ByteArray_replaceCString_withCString_(ba, "%M", s);
	
	snprintf(s, 128, "%02f", c.seconds);
	ByteArray_replaceCString_withCString_(ba, "%S", s);
	
	return ba;
}

void Duration_print(Duration *self)
{
	ByteArray *ba = Duration_asByteArrayWithFormat_(self, NULL);
	ByteArray_print(ba);
	ByteArray_free(ba);
}

// math -------------------------------------------------------- 

void Duration_add_(Duration *self, Duration *other)
{ 
	self->seconds += other->seconds; 
}

void Duration_subtract_(Duration *self, Duration *other)
{ 
	self->seconds -= other->seconds; 
}

