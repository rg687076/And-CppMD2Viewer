/**********************************************************/
/*  mqo.fsh                                               */
/*                                                        */
/*  Created by hgiashijima on 2016/11/20                  */
/*  Copyright (c) 2016年 higashijima. All rights reserved.*/
/**********************************************************/
#version 100

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D u_TexSampler;

varying vec4 v_Color;
varying vec2 v_TexCoord;

void main() {
    vec4 texcolor = texture2D(u_TexSampler, v_TexCoord);
    gl_FragColor = texcolor;
//    gl_FragColor = v_Color * texcolor;
}
