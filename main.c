#include "tinysquish.h"
#include "memory_writer.h"
#include "memory_reader.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

// static bool stdin_read(uint8_t* byte, void* private_data)
// {
// 	(void) private_data;
// 	return fread(byte, sizeof(uint8_t), 1, stdin) == 1;
// }

// static bool noop_write_size(uint32_t size, void* private_data)
// {
// 	(void) private_data;
// 	(void) size;
// 	return true;
// }

// static bool stdout_write(uint8_t byte, void* private_data)
// {
// 	(void) private_data;
// 	return fwrite(&byte, sizeof(uint8_t), 1, stdout) == 1;
// }

int main(int argc, char** argv)
{
	(void) argc; (void) argv;

	WriteInterface* memory_writer_compressed = memory_writer_new(10000);

	char* data = "uniform sampler2D d;uniform int n;out vec4 v;float i=100.;vec3 c=vec3(1.5,1.,.6),x=vec3(.4,.75,1.),y=normalize(vec3(-1.,-1.,.1));uint m=0u;void t(){m=m^m<<13u,m=m^m>>17u,m=m^m<<5u,m*=1685821657u;}void t(float v){m+=floatBitsToUint(v),t();}float s(){return t(),uintBitsToFloat(m&8388607u|1065353216u)-1.5;}vec3 f(){return vec3(s(),s(),s());}struct Co{vec3 o;vec3 d;vec3 k;int i;vec3 c;vec3 l;};Co f(vec3 v,vec3 d,vec3 y){return Co(v,d,v,0,vec3(0.),y);}float f(in vec3 v){return dot(v,v);}float f(vec3 v,vec3 d,vec3 p,vec3 y){vec3 i=y-p,x=v-p,m=p-d,z=v-d,c=d-y,k=v-y,a=cross(m,c);return sqrt(sign(dot(cross(m,a),z))+sign(dot(cross(i,a),x))+sign(dot(cross(c,a),k))<2.?min(min(f(m*clamp(dot(m,z)/f(m),0.,1.)-z),f(i*clamp(dot(i,x)/f(i),0.,1.)-x)),f(c*clamp(dot(c,k)/f(c),0.,1.)-k)):dot(a,z)*dot(a,z)/f(a));}float s(vec3 v){vec3 d=vec3(abs(v.xy),v.z+.04);if(length(d)>.8)return.8;float n=3.5;d*=n;d+=sin(d.yzx*6.)*.005;vec3 m=vec3(.03,0.,1.8),x=vec3(0.,.3,0.),y=vec3(0.,.9,.7),z=vec3(1.,0.,0.),i=vec3(1.9,0.,1.2),a=vec3(.02,.5,.4);float k=f(d,m,a,z),c=f(d,y,x,z),s=f(d,y,i,z);return(min(min(c,s),k)-.01+cos(v.x*8.)*.005)/n;}float e(vec2 v){vec2 d=vec2(v.x,abs(v.y)),k=normalize(vec2(1.)),z=vec2(k.y,-k.x);float c=dot(d,k),a=dot(d,z),m=sqrt(1.-8.*c/(1.+8.*abs(c))),i=m*(c>0.?abs(a):length(d));return sign(a)*sin(i*120.)*exp(-i*20.-c*5.);}float p(vec2 v){float i=texture(d,v*.15).x*.04+.01;return i-(e(vec2(.28,0.)-v)+e(-v)+.1*e(vec2(-.3,0.)-v))*.025;}vec2 k=vec2(.0005,0.);vec3 w(vec2 v){float z=p(v)-p(v+k.xy),y=p(v)-p(v+k.yx);return normalize(cross(vec3(k.yx,-z),vec3(k.xy,-y)));}vec3 C(vec3 v){float i=s(v);return normalize(vec3(i-s(v+k.xyy),i-s(v+k.yxy),i-s(v+k.yyx)));}void r(inout Co v){float d=.008,z=0.;for(int m=0;m<200;m++){if(length(v.o-v.k)>i)return;if(v.k.z>1.||v.k.y>3.||v.k.x>3.)return;if(v.k.z>.1&&v.k.y+v.k.x<-.5&&v.d.z>0.)return;float a=s(v.k),c=v.k.z-p(v.k.xy);if(abs(a)<k.x){v.i=2;return;}if(c<0.){v.k-=d*c/(c-z)*v.d;v.i=1;return;}d=d*1.02;v.k+=min(d*max(c*(1.-abs(v.d.z)+max(v.d.z,0.)*30.)*80.,1.),a)*v.d;z=c;}}vec3 h(vec3 v){return mix(vec3(1.71,1.31,.83),x,pow(abs(v.z),.5))+pow(max(dot(v,y),0.),1500.)*c*4.;}Co C(Co v,float d){vec3 z=-w(v.k.xy);float i=abs(dot(v.d,z));vec3 y=d*v.l*.9*(1.-i*.98),m=reflect(v.d,z);return f(v.k+z*.01,m,y);}void o(inout Co v){vec3 d=-C(v.k);float i=abs(dot(v.d,d)),m=abs(v.k.z);m=sqrt(min(m*6.+.1,1.));vec3 z=reflect(y,d),a=reflect(vec3(0.,0.,1.),d);float k=pow(max(dot(v.d,z),0.),20.)*(1.-i*.98),n=pow(max(dot(v.d,a)+.75,0.)/1.75,2.)*(1.-i*.98),p=mix(1.,s(v.k+d*.1)/.1,.3),f=s(v.k+y*.1)<s(v.k+y*.2)?1.:0.;vec3 e=vec3(.8,.3,.1);float r=dot(d,y);r=max(r,0.);v.c+=p*(r*c*e+(1.+d.z)/2.*mix(c,x,.6)*e)*m+f*k*c+n*x*m+x*.02;}Co a[4];int z=1;void l(Co v){if(z>=4)return;a[z]=v;z++;}void u(inout Co v){float d=-(dot(v.k,vec3(0.,0.,1.))-.08)/dot(v.d,vec3(0.,0.,1.));if(gl_FragCoord.y>390&&gl_FragCoord.x>684&&gl_FragCoord.x<1208&&gl_FragCoord.y<761&&(gl_FragCoord.y<598||gl_FragCoord.x>893))d=4.5;v.k+=v.d*d;a[0]=v;for(int m=0;m<4;m++){if(m>=z)break;r(a[m]);float y=a[m].i>0?pow(max(i-distance(a[m].o,a[m].k),0.)/i,2.):0.;a[m].c=(1.-y)*h(a[m].d);if(a[m].i==1)l(C(a[m],y));else if(a[m].i==2)o(a[m]);}for(int m=0;m<z;m++)v.c+=a[m].c*a[m].l;z=1;}void main(){vec2 d=(gl_FragCoord.xy-vec2(960.,540.))/vec2(960.,960.);t(d.x);t(d.y);vec3 i=vec3(0.);int m=SAMPLES+n;for(int z=0;z<m;z++){vec2 k=d+f().xy/500.;vec3 y=vec3(3.5,3.5,p(vec2(3.5,3.5))+1.5)+normalize(f())*.04,a=vec3(0.,0.,.15),c=normalize(a-y),x=vec3(0.,0.,-1.),r=normalize(cross(c,x)),e=normalize(cross(c,r)),l=(r*-k.x+e*k.y)*.3;Co C=f(y,normalize(l+c),vec3(1.));u(C);i+=C.c;}i/=float(m);i+=pow(s(),2.)*.2*vec3(.8,.9,1.);i*=1.-pow(length(d)*.7,2.);v=vec4(pow(log(i+1.),vec3(1.3))*1.25,1.);}";
	size_t data_len = strlen(data);

	assert(tinysquish_compress((uint8_t*) data, data_len, memory_writer_compressed));

	ReadInterface* memory_reader = memory_reader_from_writer(memory_writer_compressed);
	WriteInterface* memory_writer_decompressed = memory_writer_new(data_len);

	assert(tinysquish_decompress(memory_reader, memory_writer_decompressed));

	printf("input: %ld\noutput: %ld\n", data_len, memory_writer_length(memory_writer_compressed));

	assert(memory_writer_compare(memory_writer_decompressed, (uint8_t*) data, data_len));

	// ReadInterface* memory_reader_2 = memory_reader_from_writer(memory_writer_compressed);
	// WriteInterface writer = { .write_size = noop_write_size, .write = stdout_write };
	// assert(tinysquish_decompress(memory_reader_2, &writer));

	return 0;
}
