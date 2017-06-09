#include "graphics.h"

static DMA2D_HandleTypeDef dma2dHandle;

uint16_t hsv2rgb565(uint16_t h, uint8_t s, uint8_t v){
	uint8_t r, g, b;
	if((h>359)||(s>100)||(v>100)) return 0;
	if(h<61){
		r=255;
		g=(425*h)/100;
		b=0;
	}else if(h<121){
		r=255-(425*(h-60))/100;
		g=255;
		b=0;
	}else if(h<181){
		r=0;
		g=255;
		b=(425*(h-120))/100;
	}else if(h<241){
		r=0;
		g=255-(425*(h-180))/100;
		b=255;
	}else if(h<301){
		r=(425*(h-240))/100;
		g=0;
		b=255;
	}else{
		r=255;
		g=0;
		b=255-((425*(h-300))/100);
	}
	s=100-s;
	r=((r+((255-r)*s)/100)*v)/100;
	g=((g+((255-g)*s)/100)*v)/100;
	b=((b+((255-b)*s)/100)*v)/100;
	return RGB2COLOR(r,g,b);
}

void gFill(uint16_t color){
	//uint32_t i; for(i=0;i<FRAME_BUFFER_SIZE;i+=2) *((volatile uint16_t*)(fbAddr+i))=color; //igy is lehetne
	gDrawFilledRectangle(0,0,480,272,color); //DMA2D-vel
}

extern inline void gDrawPixel(uint16_t x, uint16_t y, uint16_t color); //definicio a headerben

void gDrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color){
	int16_t dx, dy, incx, incy, incx_parallel, incy_parallel, e1, e2, err, x, y, i;
	if((x1>=WIDTH)||(y1>=HEIGHT)||(x2>=WIDTH)||(y2>=HEIGHT)) return;
	dx=x2-x1;
	dy=y2-y1;
	incx=(dx>0)?1:((dx<0)?-1:0);
	incy=(dy>0)?1:((dy<0)?-1:0);
	if(dx<0) dx=-dx;
	if(dy<0) dy=-dy;
	if(dx>dy){
		incx_parallel=incx;
		incy_parallel=0;
		e1=dy;
		e2=dx;
	}
	else{
		incx_parallel=0;
		incy_parallel=incy;
		e1=dx;
		e2=dy;
	}
	x=x1;
	y=y1;
	err=e2>>1;
	gDrawPixel(x,y,color);
	for(i=0;i<e2;i++){
		err-=e1;
		if(err<0){
			err+=e2;
			x+=incx;
			y+=incy;
		}
		else{
			x+=incx_parallel;
			y+=incy_parallel;
		}
		gDrawPixel(x,y,color);
	}
}

void gDrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color){
	if(((x+w)>WIDTH)||((y+h)>HEIGHT)||(w==0)||(h==0)) return;
	gDrawFilledRectangle(x,y,1,h,color);
	gDrawFilledRectangle(x+w-1,y,1,h,color);
	gDrawFilledRectangle(x,y,w,1,color);
	gDrawFilledRectangle(x,y+h-1,w,1,color);
}

void gDrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color){
	uint32_t colorARGB8888;
	if(((x+w)>WIDTH)||((y+h)>HEIGHT)||(w==0)||(h==0)) return;
	colorARGB8888=0xFF000000|((0xF800&color)<<8)|((0x07E0&color)<<5)|((0x001F&color)<<3);
	__HAL_RCC_DMA2D_CLK_ENABLE();
	dma2dHandle.Instance=DMA2D;
	dma2dHandle.Init.Mode=DMA2D_R2M;
	dma2dHandle.Init.ColorMode=DMA2D_RGB565; //ez bugos: ARGB8888 formatumban kell megadni
	dma2dHandle.Init.OutputOffset=WIDTH-w;
	HAL_DMA2D_Init(&dma2dHandle);
	HAL_DMA2D_ConfigLayer(&dma2dHandle,currentLayer);
	HAL_DMA2D_Start(&dma2dHandle,colorARGB8888,fbAddr+2*(WIDTH*y+x),w,h);
	HAL_DMA2D_PollForTransfer(&dma2dHandle,10);
}

void gDrawCircle(uint16_t cx, uint16_t cy, uint16_t r, uint16_t color){
	int16_t d=1-r, ix=0, iy=-2*r, x=0, y=r;
	if((cx+r>=WIDTH)||(cy+r>=HEIGHT)||(r>cx)||(r>cy)||(r==0)) return;
	gDrawPixel(cx,cy+r,color);
	gDrawPixel(cx,cy-r,color);
	gDrawPixel(cx+r,cy,color);
	gDrawPixel(cx-r,cy,color);
	while(x<y){
		if(d>=0){
			y--;
			iy+=2;
			d+=iy;
		}
		x++;
		ix+=2;
		d+=ix+1;
		gDrawPixel(cx+x,cy+y,color);
		gDrawPixel(cx-x,cy+y,color);
		gDrawPixel(cx+x,cy-y,color);
		gDrawPixel(cx-x,cy-y,color);
		gDrawPixel(cx+y,cy+x,color);
		gDrawPixel(cx-y,cy+x,color);
		gDrawPixel(cx+y,cy-x,color);
		gDrawPixel(cx-y,cy-x,color);
	}
}

void gDrawFilledCircle(uint16_t cx, uint16_t cy, uint16_t r, uint16_t color){
	int32_t d=3-2*r, p=0, q=r;
	int16_t x, y, len;
	if((cx+r>=WIDTH)||(cy+r>=HEIGHT)||(r>cx)||(r>cy)||(r==0)) return;
	gDrawCircle(cx,cy,r,color);
	while(p<=q){
		if(q>0){
			x=cx-p;
			y=cy-q;
			len=2*q;
			if(y<0){len+=y; y=0;}
			gDrawFilledRectangle(x,y,1,len,color);
			x=cx+p;
			y=cy-q;
			len=2*q;
			if(y<0){len+=y; y=0;}
			gDrawFilledRectangle(x,y,1,len,color);
		}
		if(p>0){
			x=cx-q;
			y=cy-p;
			len=2*p;
			if(y<0){len+=y; y=0;}
			gDrawFilledRectangle(x,y,1,len,color);
			x=cx+q;
			y=cy-p;
			len=2*p;
			if(y<0){len+=y; y=0;}
			gDrawFilledRectangle(x,y,1,len,color);
		}
		if(d<0){
			d+=p*4+6;
		}
		else{
			d+=(p-q)*4+10;
			q--;
		}
		p++;
	}
}

void gDrawImage(image_t *img, uint16_t x, uint16_t y){
	if(((x+img->w)>WIDTH)||((y+img->h)>HEIGHT)||(img->w==0)||(img->h==0)) return;
	dma2dHandle.Instance=DMA2D;
	dma2dHandle.Init.Mode=DMA2D_M2M;
	dma2dHandle.Init.ColorMode=DMA2D_RGB565;
	dma2dHandle.Init.OutputOffset=WIDTH-img->w;
	dma2dHandle.LayerCfg[1].AlphaMode=DMA2D_NO_MODIF_ALPHA;
	dma2dHandle.LayerCfg[1].InputAlpha=0xFF;
	dma2dHandle.LayerCfg[1].InputColorMode=CM_RGB565;
	dma2dHandle.LayerCfg[1].InputOffset=0;
	HAL_DMA2D_Init(&dma2dHandle);
	HAL_DMA2D_ConfigLayer(&dma2dHandle,1); //bug: layer1 kell
	HAL_DMA2D_Start(&dma2dHandle,(uint32_t)(img->data),fbAddr+2*(WIDTH*y+x),img->w,img->h);
	HAL_DMA2D_PollForTransfer(&dma2dHandle,10);
}

uint8_t gPutChar(uint16_t x, uint16_t y, char c, const void *font, uint8_t h, uint16_t fontColor, uint16_t bgColor){
	uint16_t ix, iy, w;
	uint32_t mask, check;
	const uint16_t *ptr16;
	const uint32_t *ptr32;
	if(h<=16){ //kis betumeret --> uint16_t adattipus
		if(h!=*((const uint16_t*)font)) return 0;
		ptr16=((const uint16_t*)font)+((uint8_t)c-32)*(h+1)+1;
		w=ptr16[0];
	}
	else{ //nagy betumeret --> uint32_t adattipus
		if(h!=*((const uint32_t*)font)) return 0;
		ptr32=((const uint32_t*)font)+((uint8_t)c-32)*(h+1)+1;
		w=ptr32[0];
	}
	for(iy=0;iy<h;iy++){
		mask=0x00000001<<(w-1);
		for(ix=0;ix<w;ix++){
			check=(h<=16)?(ptr16[iy+1]&mask):(ptr32[iy+1]&mask);
			if(!check)
				gDrawPixel(x+ix,y+iy,bgColor);
			else
				gDrawPixel(x+ix,y+iy,fontColor);
			mask>>=1;
		}
	}
	return w;
}

void gPutString(uint16_t x, uint16_t y, char *str, const void *font, uint8_t h, uint16_t fontColor, uint16_t bgColor){
	uint8_t w;
	uint16_t pos=x;
	while(*str){
		w=gPutChar(pos,y,*str,font,h,fontColor,bgColor);
		pos+=w;
		str++;
	}
}
