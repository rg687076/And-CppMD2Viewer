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

varying vec4 v_Color;

void main() {
    gl_FragColor = v_Color;
}
