uniform sampler2D u_Texture;

varying vec2 v_UV;

/**************************************************
NegativeFilter
Description: Sets the negative of image by substracting to
255 the value of each color in the pixel.
Arguments:  Void
Return: Void
Date: 2015/02/24
Author: Alvaro Chavez Mixco
***************************************************/
//void CBMPImage::negativeFilter()
//{
//    int counter = 0;
//    for (int row = 0; row < m_BIH.biHeight; row++)  // do all rows
//    {
//        counter = m_pitch * row; // m_pitch is bytes, so we are calculating which index will be at the start of each row
//        for (int col = 0; col < m_BIH.biWidth; col++)
//        {
//            //Substract  each color to 255 to get the negative
//            m_imageData[counter] = 255 - m_imageData[counter];
//            m_imageData[counter + 1] = 255 - m_imageData[counter + 1];
//            m_imageData[counter + 2] = 255 - m_imageData[counter + 2];

//            counter += 4;
//        }
//    }
//}

void main()
{
	vec4 originalColor=texture2D( u_Texture, v_UV );//Get the original color from the texture
	vec4 negativeColor;

	//Get the negative color, by substracting 1 (100 percent) from the current value of the color
	negativeColor.x=1.0f-originalColor.x;
	negativeColor.y=1.0f-originalColor.y;
	negativeColor.z=1.0f-originalColor.z;

	gl_FragColor = vec4(negativeColor.x,negativeColor.y,negativeColor.z,originalColor.w);//Don't change alpha value
	//gl_FragColor = vec4(0,1,0,1);
}
