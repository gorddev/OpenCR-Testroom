#version 410 core
precision mediump float;

in vec3 vPos;

uniform vec2 uResolution;
uniform vec2 uMousepos;

out vec4 color;

const int iteration = 60;
const float scale = 3.0;

vec2 complex_sqr(vec2 c1) {
    return vec2(
        c1.x * c1.x - c1.y*c1.y,
        2.0 * c1.x * c1.y
    );
}

vec4 mandelbrot(vec2 frag) {
    vec2 z = vec2(0.0);
    vec2 c = frag;

    int i;
    for(i = 0; i < iteration; i++) {
        z = complex_sqr(z) + c;
        if (length(z) >= 2) {
            break;
        }
    }

    if (length(z) < 2) {
        return vec4(1.0);
    } else {
        return vec4(float(i)/float(iteration));
    }
}


vec4 julia(vec2 pixel_coord) {
    vec2 z = pixel_coord;
    vec2 c = uMousepos;

    int i;
    for(i = 0; i < iteration; i++) {
        z = complex_sqr(z) + c;
        if (dot(z, z) > 4.0) {
            break;
        }
    }
    if (i == iteration) {
        return vec4(0.0, 0.0, 0.0, 1.0);
    } else {
        float t = float(i) / float(iteration);
        return vec4(t, t, t, 1.0);
    }
}

void main() {
    vec2 uv = (gl_FragCoord.xy - 0.5 * uResolution.xy) / uResolution.y;
    color = julia(uv * 3.0);
}
