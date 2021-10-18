
uniform sampler2D u_Texture;
varying vec2 v_UV;
/**************************************************
sepiaTone()
Description: Sets the sepia Tone using the formulas:
outputRed becomes  (inputRed * .393) + (inputGreen *.769) + (inputBlue * .189)
outputGreen becomes (inputRed * .349) + (inputGreen *.686) + (inputBlue * .168)
outputBlue becomes (inputRed * .272) + (inputGreen *.534) + (inputBlue * .131)
Arguments:  Void
Return: Void
Date: 2015/02/24
Author: Alvaro Chavez Mixco
***************************************************/
//void CBMPImage::sepiaTone()
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

//            //use given formulas to convert the color into a sepia tone
//            double outputBlue = (inputRed * .272) + (inputGreen *.534) + (inputBlue * .131);
//            if (outputBlue > 255)//check value isn't bigger than 255
//            {
//                outputBlue = 255;
//            }

//            double outputGreen = (inputRed * .349) + (inputGreen *.686) + (inputBlue * .168);
//            if (outputGreen > 255)
//            {
//                outputGreen = 255;
//            }

//            double outputRed = (inputRed * .393) + (inputGreen *.769) + (inputBlue * .189);
//            if (outputRed > 255)
//            {
//                outputRed = 255;
//            }

//            //Assign the output to each individual color
//            m_imageData[counter] = outputBlue;
//            m_imageData[counter + 1] = outputGreen;
//            m_imageData[counter + 2] = outputRed;

//            counter += 4;
//        }
//    }

//}

void main()
{
    vec4 originalColor=texture2D( u_Texture, v_UV );//Get original texture colors   
	vec4 sepiaColor;

    //Convert to sepia tones, using formula
	//	outputRed becomes  (inputRed * .393) + (inputGreen *.769) + (inputBlue * .189)
	//outputGreen becomes (inputRed * .349) + (inputGreen *.686) + (inputBlue * .168)
	//outputBlue becomes (inputRed * .272) + (inputGreen *.534) + (inputBlue * .131)

	sepiaColor.x = (originalColor.x * 0.272) + (originalColor.y *0.534) + (originalColor.z * 0.131);
    sepiaColor.y = (originalColor.x * 0.349) + (originalColor.y *0.686) + (originalColor.z * 0.168);
    sepiaColor.z = (originalColor.x * 0.393) + (originalColor.y *0.769) + (originalColor.z * 0.189);


    gl_FragColor = vec4(sepiaColor.x,sepiaColor.y,sepiaColor.z,originalColor.w);
	//gl_FragColor = vec4(1,originalColor.y,0,1);
}

