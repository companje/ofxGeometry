/*
 *  ofxGeometry.cpp
 *
 *  Created by Rick Companje on 23-03-11.
 *  Copyright 2010 Globe4D. All rights reserved.
 *
 */

#include "ofxGeometry.h"

ofxCartesian& ofxCartesian::operator=(ofxLatLon ll) {
	//from ofxLatLon to ofxCartesian
	float phi = ofDegToRad(ll.lat + 90);
	float theta = ofDegToRad(ll.lon - 90);
	x =  sin(phi) * cos(theta);
	y =  sin(phi) * sin(theta);
	z = -cos(phi);
	if (ll.lat>=0) x=-x;		
	return *this;
}

ofxQuaternionExtra& ofxQuaternionExtra::operator=(ofxLatLon ll) {	
	//from ofxLatLon to ofxQuaternionExtra
	return *this = ofxCartesian(ll);
}

ofxQuaternionExtra& ofxQuaternionExtra::operator=(ofxCartesian cp) {
	//from ofxCartesian to ofxQuaternionExtra
	*this = ofxMatrix4x4(0,0,0,0, 0,0,0,0, -cp.x,-cp.y,-cp.z,0, 0,0,0,1);
	return *this;
}

ofxLatLon& ofxLatLon::operator=(ofxQuaternionExtra q) {
	//from ofxQuaternionExtra to ofxLatLon
	ofxCartesian c(q);
	lat = ofRadToDeg(asin(c.z));
	lon = ofRadToDeg(atan2(c.y,c.x))-90;
	if (lon<-180) lon+=360;
	return *this;
};

ofxCartesian& ofxCartesian::operator=(ofxQuaternion q) {
	//from ofxQuaternionExtra to ofxCartesian
	ofxVec4f v(0,0,-1,0);
	ofxMatrix4x4 m;
	q.get(m);
	ofxVec4f mv = m*v;
	set(mv.x,mv.y,mv.z);
	return *this;
}

//from quaternion to axis angle
ofxAxisAngle& ofxAxisAngle::operator=(ofxQuaternionExtra q) {
	q.getRotate(this->angle, this->axis);
	return *this;
}

void ofxQuaternionExtra::rotate(ofxAxisAngle aa) {
	(*this) *= ofxQuaternion(aa.angle,aa.axis);
}

void ofxQuaternionExtra::rotateX(float angle) { //degrees?
	rotate(ofxAxisAngle(1,0,0,angle));
}

void ofxQuaternionExtra::rotateY(float angle) {
	rotate(ofxAxisAngle(0,1,0,angle));
}

void ofxQuaternionExtra::rotateZ(float angle) {
	rotate(ofxAxisAngle(0,0,1,angle));
}

ostream& operator <<(ostream &os,const ofxLatLon ll) {
	os << ll.lat << "," << ll.lon; 
	return os;
}

ostream& operator <<(ostream &os,const ofxCartesian c) {
	os << c.x << "," << c.y << "," << c.z; 
	return os;
}

ofxLatLon::ofxLatLon(float lat, float lon) {
	this->lat = lat;
	this->lon = lon;
}

ofxLatLon::ofxLatLon(ofxQuaternionExtra q) {
	*this = q;
}

ofxQuaternionExtra& ofxQuaternionExtra::operator=(ofxQuaternion q) {
	set(q);
	return *this;
}

float ofxQuaternionExtra::getDistance(ofxLatLon ll) {
	ofxLatLon ll2;
	float lat1 = ll.lat;
	float lon1 = ll.lon;
	float lat2 = ll2.lat;
	float lon2 = ll2.lon;
	float R = 1; //6371; // km
	float dLat = ofDegToRad(lat2-lat1);
	float dLon = ofDegToRad(lon2-lon1);
	float a = sin(dLat/2) * sin(dLat/2) + cos(ofDegToRad(lat1)) * cos(ofDegToRad(lat2)) * sin(dLon/2) * sin(dLon/2); 
	float c = 2 * atan2(sqrt(a), sqrt(1-a)); 
	float d = R * c;
	return d;
}

