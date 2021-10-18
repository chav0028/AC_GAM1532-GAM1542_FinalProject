
uniform sampler2D u_Texture;

varying vec2 v_UV;


/**************************************************
blackWhiteFilter()
Description: Sets the image as black and white by averaging
the value of all 3  colours,
Arguments:  Void
Return: Void
Date: 2015/02/24
Author: Alvaro Chavez Mixco
***************************************************/
//void CBMPImage::blackWhiteFilter()
//{
//    int counter = 0;
//    for (int row = 0; row < m_BIH.biHeight; row++)  // do all rows
//    {
//        counter = m_pitch * row; // m_pitch is bytes, so we are calculating which index will be at the start of each row
//        for (int col = 0; col < m_BIH.biWidth; col++)
//        {
//            //cast the value of the color into char
//            unsigned char inputBlue = (unsigned char)m_imageData[counter];
//            unsigned char inputGreen = (unsigned char)m_imageData[counter + 1];
//            unsigned char inputRed = (unsigned char)m_imageData[counter + 2];

//            //Get the average color->Black and white
//            unsigned char averageColor = (inputBlue + inputRed + inputGreen) / 3;

//            //Assign the average color to each individual color
//            m_imageData[counter] = averageColor;
//            m_imageData[counter + 1] = averageColor;
//            m_imageData[counter + 2] = averageColor;

//            counter += 4;
//        }
//    }ss
//}


void main()
{
	vec4 originalColor=texture2D( u_Texture, v_UV );//Get the original color from the texture
	float averageColor=(originalColor.x+originalColor.y+originalColor.y)/3.0f;//Calculate the black/white amount by adding the RGB values and dividing them by 3

	gl_FragColor = vec4(averageColor,averageColor,averageColor,originalColor.w);//Don't change the alpha value

}
