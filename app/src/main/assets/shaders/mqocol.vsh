/**********************************************************/
/*  mqo.vsh                                               */
/*                                                        */
/*  Created by hgiashijima on 2016/11/20                  */
/*  Copyright (c) 2016年 higashijima. All rights reserved.*/
/**********************************************************/
#version 100

attribute vec4 a_Vertex;
//attribute vec4 a_Normal;
attribute vec4 a_Color;

uniform mat4 u_MvpMatrix;
//uniform mat4 u_NormalMatrix;

varying vec4 v_Color;

void main() {
	vec3 lightDirection	= vec3(-0.35, 0.35, 0.87);
	gl_Position		= u_MvpMatrix * a_Vertex;

//	vec3	normal	= normalize(vec3(u_NormalMatrix * a_Normal));
//	float	nDotL	= max(dot(normal, lightDirection), 0.0);
//			v_Color	= vec4(a_Color.rgb * nDotL, a_Color.a);
	v_Color	= a_Color;
}
