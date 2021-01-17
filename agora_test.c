
#define AGORA_TEST_PRT(fmt...)   \
    do {\
        printf("[%s]-%d: ", __FUNCTION__, __LINE__);\
        printf(fmt);\
       }while(0)

	
#define LOGS(fmt, ...)          fprintf(stdout, "" fmt "\n", ##__VA_ARGS__)
#define LOGI(fmt, ...)          fprintf(stdout, "I/ " fmt " at %s:%u\n", ##__VA_ARGS__, __PRETTY_FUNCTION__, __LINE__)
#define LOGD(fmt, ...)          fprintf(stdout, "D/ " fmt " at %s:%u\n", ##__VA_ARGS__, __PRETTY_FUNCTION__, __LINE__)
#define LOGE(fmt, ...)          fprintf(stdout, "E/ " fmt " at %s:%u\n", ##__VA_ARGS__, __PRETTY_FUNCTION__, __LINE__)
#define LOGW(fmt, ...)          fprintf(stdout, "W/ " fmt " at %s:%u\n", ##__VA_ARGS__, __PRETTY_FUNCTION__, __LINE__)


#define TAG_APP     "[app]"
#define TAG_API     "[api]"
#define TAG_EVENT   "[event]"

char datachannel[32];

typedef struct tagaogra_test_vi_param_s
{
    HI_S32 s32ViDevCnt;        // VI Dev Total Count    
    HI_S32 s32ViDevInterval;   // Vi Dev Interval    
    HI_S32 s32ViChnCnt;        // Vi Chn Total Count    
    HI_S32 s32ViChnInterval;   // VI Chn Interval
}AGORA_TEST_VI_PARAM_S;


HI_S32 Agora_Sys_GetPicSize(VIDEO_NORM_E enNorm, PIC_SIZE_E enPicSize, SIZE_S *pstSize)
{
    switch (enPicSize)
    {
        case PIC_QCIF:
            pstSize->u32Width = D1_WIDTH / 4;
            pstSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?144:120;
            break;
        case PIC_CIF:
            pstSize->u32Width = D1_WIDTH / 2;
            pstSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?288:240;
            break;
        case PIC_D1:
            pstSize->u32Width = D1_WIDTH;
            pstSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
            break;
        case PIC_960H:
            pstSize->u32Width = 960;
            pstSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
            break;
        case PIC_2CIF:
            pstSize->u32Width = D1_WIDTH / 2;
            pstSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
            break;
        case PIC_QVGA:    /* 320 * 240 */
            pstSize->u32Width = 320;
            pstSize->u32Height = 240;
            break;
        case PIC_VGA:     /* 640 * 480 */
            pstSize->u32Width = 640;
            pstSize->u32Height = 480;
            break;
        case PIC_XGA:     /* 1024 * 768 */
            pstSize->u32Width = 1024;
            pstSize->u32Height = 768;
            break;
        case PIC_SXGA:    /* 1400 * 1050 */
            pstSize->u32Width = 1400;
            pstSize->u32Height = 1050;
            break;
        case PIC_UXGA:    /* 1600 * 1200 */
            pstSize->u32Width = 1600;
            pstSize->u32Height = 1200;
            break;
        case PIC_QXGA:    /* 2048 * 1536 */
            pstSize->u32Width = 2048;
            pstSize->u32Height = 1536;
            break;
        case PIC_WVGA:    /* 854 * 480 */
            pstSize->u32Width = 854;
            pstSize->u32Height = 480;
            break;
        case PIC_WSXGA:   /* 1680 * 1050 */
            pstSize->u32Width = 1680;
            pstSize->u32Height = 1050;
            break;
        case PIC_WUXGA:   /* 1920 * 1200 */
            pstSize->u32Width = 1920;
            pstSize->u32Height = 1200;
            break;
        case PIC_WQXGA:   /* 2560 * 1600 */
            pstSize->u32Width = 2560;
            pstSize->u32Height = 1600;
            break;
        case PIC_HD720:   /* 1280 * 720 */
            pstSize->u32Width = 1280;
            pstSize->u32Height = 720;
            break;
        case PIC_HD1080:  /* 1920 * 1080 */
            pstSize->u32Width = 1920;
            pstSize->u32Height = 1080;
            break;
        default:
            return HI_FAILURE;
    }
    return HI_SUCCESS;
}


HI_U32 Agora_Sys_CalcPicVbBlkSize(VIDEO_NORM_E enNorm, PIC_SIZE_E enPicSize, PIXEL_FORMAT_E enPixFmt, HI_U32 u32AlignWidth,COMPRESS_MODE_E enCompFmt)
{
    HI_S32 s32Ret      		= HI_FAILURE;
    SIZE_S stSize;
    HI_U32 u32Width 		= 0;
    HI_U32 u32Height 		= 0;
    HI_U32 u32BlkSize 		= 0;
	HI_U32 u32HeaderSize 	= 0;

    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enNorm, enPicSize, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("get picture size[%d] failed!\n", enPicSize);
            return HI_FAILURE;
    }

    if (PIXEL_FORMAT_YUV_SEMIPLANAR_422 != enPixFmt && PIXEL_FORMAT_YUV_SEMIPLANAR_420 != enPixFmt)
    {
        SAMPLE_PRT("pixel format[%d] input failed!\n", enPixFmt);
            return HI_FAILURE;
    }

    if (16!=u32AlignWidth && 32!=u32AlignWidth && 64!=u32AlignWidth)
    {
        SAMPLE_PRT("system align width[%d] input failed!\n",\
               u32AlignWidth);
            return HI_FAILURE;
    }
    if (704 == stSize.u32Width)
    {
        stSize.u32Width = 720;
    }
    //SAMPLE_PRT("w:%d, u32AlignWidth:%d\n", CEILING_2_POWER(stSize.u32Width,u32AlignWidth), u32AlignWidth);

    u32Width  = CEILING_2_POWER(stSize.u32Width, u32AlignWidth);
    u32Height = CEILING_2_POWER(stSize.u32Height,u32AlignWidth);

    if (PIXEL_FORMAT_YUV_SEMIPLANAR_422 == enPixFmt)
    {
        u32BlkSize = u32Width * u32Height * 2;
    }
    else
    {
        u32BlkSize = u32Width * u32Height * 3 / 2;
    }


	if(COMPRESS_MODE_SEG == enCompFmt)
	{
		VB_PIC_HEADER_SIZE(u32Width,u32Height,enPixFmt,u32HeaderSize);
	}

	u32BlkSize += u32HeaderSize;

    return u32BlkSize;
}

HI_S32 MPP_Sys_Init(VB_CONF_S *pstVbConf)
{
    MPP_SYS_CONF_S stSysConf = {0};
    HI_S32 s32Ret = HI_FAILURE;
    HI_S32 i;

    HI_MPI_SYS_Exit();

    for(i=0;i<VB_MAX_USER;i++)
    {
         HI_MPI_VB_ExitModCommPool(i);
    }
    for(i=0; i<VB_MAX_POOLS; i++)
    {
         HI_MPI_VB_DestroyPool(i);
    }
    HI_MPI_VB_Exit();

    if (NULL == pstVbConf)
    {
        SAMPLE_PRT("input parameter is null, it is invaild!\n");
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_VB_SetConf(pstVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VB_SetConf failed!\n");
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_VB_Init();
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VB_Init failed!\n");
        return HI_FAILURE;
    }

    stSysConf.u32AlignWidth = SAMPLE_SYS_ALIGN_WIDTH;
    s32Ret = HI_MPI_SYS_SetConf(&stSysConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_SYS_SetConf failed\n");
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_SYS_Init();
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_SYS_Init failed!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}



	
HI_S32 Agora_Vi_Capture(SAMPLE_VI_MODE_E enViMode, VIDEO_NORM_E enNorm)
{
	VI_DEV ViDev;
	VI_CHN ViChn;
	HI_S32 i;
	HI_S32 s32Ret;
	SAMPLE_VI_PARAM_S stViParam;
	SIZE_S stTargetSize;
	RECT_S stCapRect;
	VI_CHN_BIND_ATTR_S stChnBindAttr;
	
	/*** get parameter from Sample_Vi_Mode ***/
	s32Ret = SAMPLE_COMM_VI_Mode2Param(enViMode, &stViParam);
	if (HI_SUCCESS !=s32Ret)
	{
		SAMPLE_PRT("vi get param failed!\n");
		return HI_FAILURE;
	}
	s32Ret = SAMPLE_COMM_VI_Mode2Size(enViMode, enNorm, &stCapRect, &stTargetSize);
	if (HI_SUCCESS !=s32Ret)
	{
		SAMPLE_PRT("vi get size failed!\n");
		return HI_FAILURE;
	}
	
	/*** Start AD ***/
	s32Ret = SAMPLE_COMM_VI_ADStart(enViMode, enNorm);
	if (HI_SUCCESS !=s32Ret)
	{
		SAMPLE_PRT("Start AD failed!\n");
		return HI_FAILURE;
	}
	
	/*** Start VI Dev ***/
	for(i=0; i<stViParam.s32ViDevCnt; i++)
	{
		ViDev = i * stViParam.s32ViDevInterval;
		s32Ret = SAMPLE_COMM_VI_StartDev(ViDev, enViMode);
		if (HI_SUCCESS != s32Ret)
		{
			SAMPLE_PRT("SAMPLE_COMM_VI_StartDev failed with %#x\n", s32Ret);
			return HI_FAILURE;
		}
	}
	
	/*** Start VI Chn ***/
	for(i=0; i<stViParam.s32ViChnCnt; i++)
	{
		ViChn = i * stViParam.s32ViChnInterval;

		if (SAMPLE_VI_MODE_16_1080P == enViMode
		 || SAMPLE_VI_MODE_16_720P == enViMode)
		{
			/* When in the 16x1080p mode, bind chn 2,6,10,14 to way1 is needed */
			if (ViChn%4 != 0)
			{
				s32Ret = HI_MPI_VI_GetChnBind(ViChn, &stChnBindAttr);
				if (HI_ERR_VI_FAILED_NOTBIND == s32Ret)
				{
					stChnBindAttr.ViDev = ViChn/4;
					stChnBindAttr.ViWay = 1;
					s32Ret = HI_MPI_VI_BindChn(ViChn, &stChnBindAttr);
					if (HI_SUCCESS != s32Ret)
					{
						SAMPLE_PRT("call HI_MPI_VI_BindChn failed with %#x\n", s32Ret);
						return HI_FAILURE;
					} 
				} 
			}
		}

		s32Ret = SAMPLE_COMM_VI_StartChn(ViChn, &stCapRect, &stTargetSize, enViMode, VI_CHN_SET_NORMAL);
		if (HI_SUCCESS != s32Ret)
		{
			SAMPLE_PRT("call SAMPLE_COMM_VI_StarChn failed with %#x\n", s32Ret);
			return HI_FAILURE;
		} 
	}

	return HI_SUCCESS;
}

HI_S32 Agora_VPSS_Start(HI_S32 s32GrpCnt, SIZE_S *pstSize, HI_S32 s32ChnCnt,VPSS_GRP_ATTR_S *pstVpssGrpAttr)
{
    VPSS_GRP VpssGrp;
    VPSS_CHN VpssChn;
    VPSS_GRP_ATTR_S stGrpAttr = {0};
    VPSS_CHN_ATTR_S stChnAttr = {0};
    VPSS_GRP_PARAM_S stVpssParam = {0};
    HI_S32 s32Ret;
    HI_S32 i, j;

    /*** Set Vpss Grp Attr ***/

    if(NULL == pstVpssGrpAttr)
    {
        stGrpAttr.u32MaxW = pstSize->u32Width;
        stGrpAttr.u32MaxH = pstSize->u32Height;
        stGrpAttr.enPixFmt = SAMPLE_PIXEL_FORMAT;
        
        stGrpAttr.bIeEn = HI_FALSE;
        stGrpAttr.bNrEn = HI_TRUE;
        stGrpAttr.bDciEn = HI_FALSE;
        stGrpAttr.bHistEn = HI_FALSE;
        stGrpAttr.bEsEn = HI_FALSE;
        stGrpAttr.enDieMode = VPSS_DIE_MODE_NODIE;
    }
    else
    {
        memcpy(&stGrpAttr,pstVpssGrpAttr,sizeof(VPSS_GRP_ATTR_S));
    }
    

    {
        VpssGrp = i;
        /*** create vpss group ***/
        s32Ret = HI_MPI_VPSS_CreateGrp(VpssGrp, &stGrpAttr);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("HI_MPI_VPSS_CreateGrp failed with %#x!\n", s32Ret);
            return HI_FAILURE;
        }

        /*** set vpss param ***/
        s32Ret = HI_MPI_VPSS_GetGrpParam(VpssGrp, &stVpssParam);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("failed with %#x!\n", s32Ret);
            return HI_FAILURE;
        }
        
        stVpssParam.u32IeStrength = 0;
        s32Ret = HI_MPI_VPSS_SetGrpParam(VpssGrp, &stVpssParam);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("failed with %#x!\n", s32Ret);
            return HI_FAILURE;
        }

        /*** enable vpss chn, with frame ***/
        {
            VpssChn = j;
            /* Set Vpss Chn attr */
            stChnAttr.bSpEn = HI_FALSE;
            stChnAttr.bUVInvert = HI_FALSE;
            stChnAttr.bBorderEn = HI_TRUE;
            stChnAttr.stBorder.u32Color = 0xffffff;
            stChnAttr.stBorder.u32LeftWidth = 2;
            stChnAttr.stBorder.u32RightWidth = 2;
            stChnAttr.stBorder.u32TopWidth = 2;
            stChnAttr.stBorder.u32BottomWidth = 2;
            
            s32Ret = HI_MPI_VPSS_SetChnAttr(VpssGrp, VpssChn, &stChnAttr);
            if (s32Ret != HI_SUCCESS)
            {
                SAMPLE_PRT("HI_MPI_VPSS_SetChnAttr failed with %#x\n", s32Ret);
                return HI_FAILURE;
            }
    
            s32Ret = HI_MPI_VPSS_EnableChn(VpssGrp, VpssChn);
            if (s32Ret != HI_SUCCESS)
            {
                SAMPLE_PRT("HI_MPI_VPSS_EnableChn failed with %#x\n", s32Ret);
                return HI_FAILURE;
            }
        }
        
        /*** start vpss group ***/
        s32Ret = HI_MPI_VPSS_StartGrp(VpssGrp);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("HI_MPI_VPSS_StartGrp failed with %#x\n", s32Ret);
            return HI_FAILURE;
        }

    }
    return HI_SUCCESS;
}


/*****************************************************************************
* function : Vi chn bind vpss group
*****************************************************************************/
HI_S32 Agora_Vi_BindVpss(HI_VOID)
{
    HI_S32 j, s32Ret;
    VPSS_GRP VpssGrp;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;
    AGORA_TEST_VI_PARAM_S stViParam;
    AGORA_TEST_VI_PARAM_S *pstViParam = &stViParam;
    VI_CHN ViChn;

	pstViParam->s32ViDevCnt = 8;
	pstViParam->s32ViDevInterval = 1;
	pstViParam->s32ViChnCnt = 16;
	pstViParam->s32ViChnInterval = 2;
    
    VpssGrp = 0;
    for (j=0; j<stViParam.s32ViChnCnt; j++)
    {
        ViChn = j * stViParam.s32ViChnInterval;
        
        stSrcChn.enModId = HI_ID_VIU;
        stSrcChn.s32DevId = 0;
        stSrcChn.s32ChnId = ViChn;
    
        stDestChn.enModId = HI_ID_VPSS;
        stDestChn.s32DevId = VpssGrp;
        stDestChn.s32ChnId = 0;
    
        s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDestChn);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("failed with %#x!\n", s32Ret);
            return HI_FAILURE;
        }
        
        VpssGrp ++;
    }
    return HI_SUCCESS;
}


/******************************************************************************
* funciton : Start venc stream mode (h264, mjpeg)
* note      : rate control parameter need adjust, according your case.
******************************************************************************/
HI_S32 Agora_Enc_Start(VENC_CHN VencChn, PAYLOAD_TYPE_E enType, VIDEO_NORM_E enNorm, PIC_SIZE_E enSize, SAMPLE_RC_E enRcMode, HI_U32  u32Profile)
{
    HI_S32 s32Ret;
    VENC_CHN_ATTR_S stVencChnAttr;
    VENC_ATTR_H264_S stH264Attr;
    VENC_ATTR_H264_CBR_S    stH264Cbr;
    VENC_ATTR_H264_VBR_S    stH264Vbr;
	VENC_ATTR_H264_AVBR_S    stH264AVbr;
    VENC_ATTR_H264_FIXQP_S  stH264FixQp;
    VENC_ATTR_H265_S        stH265Attr;
    VENC_ATTR_H265_CBR_S    stH265Cbr;
    VENC_ATTR_H265_VBR_S    stH265Vbr;
    VENC_ATTR_H265_AVBR_S    stH265AVbr;
    VENC_ATTR_H265_FIXQP_S  stH265FixQp;
    VENC_ATTR_MJPEG_S stMjpegAttr;
    VENC_ATTR_MJPEG_FIXQP_S stMjpegeFixQp;
    VENC_ATTR_JPEG_S stJpegAttr;
    SIZE_S stPicSize;
    s32Ret = Agora_Sys_GetPicSize(enNorm, enSize, &stPicSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Get picture size failed!\n");
        return HI_FAILURE;
    }

	stH264Attr.u32MaxPicWidth = stPicSize.u32Width;
	stH264Attr.u32MaxPicHeight = stPicSize.u32Height;
	stH264Attr.u32PicWidth = stPicSize.u32Width;/*the picture width*/
	stH264Attr.u32PicHeight = stPicSize.u32Height;/*the picture height*/			
	stH264Attr.u32BufSize  = stPicSize.u32Width * stPicSize.u32Height * 2;/*stream buffer size*/
	stH264Attr.u32Profile  = u32Profile;/*0: baseline; 1:MP; 2:HP;	3:svc_t */
	stH264Attr.bByFrame = HI_TRUE;/*get stream mode is slice mode or frame mode?*/
	//stH264Attr.u32BFrameNum = 0;/* 0: not support B frame; >=1: number of B frames */
	//stH264Attr.u32RefNum = 1;/* 0: default; number of refrence frame*/
	memcpy(&stVencChnAttr.stVeAttr.stAttrH264e, &stH264Attr, sizeof(VENC_ATTR_H264_S));
	stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
	stH264Cbr.u32Gop			= (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
	stH264Cbr.u32StatTime		= 1; /* stream rate statics time(s) */
	stH264Cbr.u32SrcFrmRate 	 = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30; /* input (vi) frame rate */
	stH264Cbr.fr32DstFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30; /* target frame rate */
	stH264Cbr.u32BitRate = 1024 * 4;
	
	stH264Cbr.u32FluctuateLevel = 1; /* average bit rate */
	memcpy(&stVencChnAttr.stRcAttr.stAttrH264Cbr, &stH264Cbr, sizeof(VENC_ATTR_H264_CBR_S));


    /******************************************
     step 1:  Create Venc Channel    
     ******************************************/
    stVencChnAttr.stVeAttr.enType = enType;
    
    stVencChnAttr.stGopAttr.enGopMode  = VENC_GOPMODE_NORMALP;
    stVencChnAttr.stGopAttr.stNormalP.s32IPQpDelta = 0;
    s32Ret = HI_MPI_VENC_CreateChn(VencChn, &stVencChnAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VENC_CreateChn [%d] faild with %#x! ===\n", \
                   VencChn, s32Ret);
        return s32Ret;
    }    
    s32Ret = HI_MPI_VENC_StartRecvPic(VencChn);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VENC_StartRecvPic faild with%#x! \n", s32Ret);
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}



/******************************************************************************
* function : venc bind vpss
******************************************************************************/
HI_S32 Agora_Enc_BindVpss(VENC_CHN VeChn, VPSS_GRP VpssGrp, VPSS_CHN VpssChn)
{
    HI_S32 s32Ret = HI_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;
    stSrcChn.enModId = HI_ID_VPSS;
    stSrcChn.s32DevId = VpssGrp;
    stSrcChn.s32ChnId = VpssChn;
    stDestChn.enModId = HI_ID_VENC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = VeChn;
    s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
    return s32Ret;
}

HI_S32 Agora_VENC_SendStream(VENC_STREAM_S* pstStream)
{
    HI_S32 s32Ret;
	video_frame_info_t stVFrameInfo;
	H264E_NALU_ISLICE = 2,						   /*I SLICE types*/
	H264E_NALU_IDRSLICE = 5,					   /*IDR SLICE types*/
	H264E_NALU_SEI	  = 6,						   /*SEI types*/
	H264E_NALU_SPS	  = 7,						   /*SPS types*/
	H264E_NALU_PPS	  = 8,						   /*PPS types*/

	if ((H264E_NALU_ISLICE == pstStream->pstPack->DataType.enH264EType)
		|| (H264E_NALU_SEI == pstStream->pstPack->DataType.enH264EType)
		|| (H264E_NALU_SPS == pstStream->pstPack->DataType.enH264EType)
		|| (H264E_NALU_PPS == pstStream->pstPack->DataType.enH264EType))
	{
		stVFrameInfo.type = VIDEO_FRAME_KEY;
	}
	else
	{
		stVFrameInfo.type = VIDEO_FRAME_DELTA;
	}
	
	stVFrameInfo.codec = VIDEO_CODEC_H264;
	stVFrameInfo.frames_per_sec = VIDEO_FRAME_RATE_FPS_30;

	s32Ret = agora_rtc_send_video_data("AgoraHiTest", 0, pstStream->pstPack->pu8Addr, pstStream->pstPack->u32Len, &stVFrameInfo);
    return s32Ret;
}



/******************************************************************************
* funciton : get stream from each channels and save them
******************************************************************************/
HI_VOID* Agora_Enc_GetVencStreamProc(HI_VOID* p)
{
    VENC_CHN_ATTR_S stVencChnAttr;
    SAMPLE_VENC_GETSTREAM_PARA_S* pstPara;
    HI_S32 maxfd = 0;
    struct timeval TimeoutVal;
    fd_set read_fds;
    HI_S32 VencFd;
    char szFilePostfix[10];
    VENC_CHN_STAT_S stStat;
    VENC_STREAM_S stStream;
    HI_S32 s32Ret;
    VENC_CHN VencChn;
    VENC_STREAM_BUF_INFO_S stStreamBufInfo;

    prctl(PR_SET_NAME, "hi_GetStream", 0, 0, 0);

    pstPara = (SAMPLE_VENC_GETSTREAM_PARA_S*)p;
    /******************************************
     step 1:  check & prepare save-file & venc-fd
    ******************************************/
	
    /* decide the stream file name, and open file to save stream */
    VencChn = 0;
    s32Ret = HI_MPI_VENC_GetChnAttr(VencChn, &stVencChnAttr);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("HI_MPI_VENC_GetChnAttr chn[%d] failed with %#x!\n", \
                   VencChn, s32Ret);
        return NULL;
    }
	/* Set Venc Fd. */
    VencFd = HI_MPI_VENC_GetFd(i);
    if (VencFd < 0)
    {
        SAMPLE_PRT("HI_MPI_VENC_GetFd failed with %#x!\n",
                   VencFd);
        return NULL;
    }
    if (maxfd <= VencFd)
    {
        maxfd = VencFd;
    }

    s32Ret = HI_MPI_VENC_GetStreamBufInfo (0, &stStreamBufInfo);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VENC_GetStreamBufInfo failed with %#x!\n", s32Ret);
        return (void *)HI_FAILURE;
    }

    /******************************************
     step 2:  Start to get streams of each channel.    
     ******************************************/
    while (HI_TRUE == pstPara->bThreadStart)
    {
        FD_ZERO(&read_fds);
        FD_SET(VencFd, &read_fds);
        TimeoutVal.tv_sec  = 2;
        TimeoutVal.tv_usec = 0;
        s32Ret = select(maxfd + 1, &read_fds, NULL, NULL, &TimeoutVal);
        if (s32Ret < 0)
        {
            SAMPLE_PRT("select failed!\n");
            break;
        }
        else if (s32Ret == 0)
        {
            SAMPLE_PRT("get venc stream time out, exit thread\n");
            continue;
        }
        else
        {
            if (FD_ISSET(VencFd, &read_fds))
            {
                /*******************************************************
                step 2.1 : query how many packs in one-frame stream.                    
                *******************************************************/
                memset(&stStream, 0, sizeof(stStream));
                s32Ret = HI_MPI_VENC_Query(i, &stStat);
                if (HI_SUCCESS != s32Ret)
                {
                    SAMPLE_PRT("HI_MPI_VENC_Query chn[%d] failed with %#x!\n", i, s32Ret);
                    break;
                }										
                /*******************************************************
				step 2.2 :suggest to check both u32CurPacks and u32LeftStreamFrames at the same time,for example:					 if(0 == stStat.u32CurPacks || 0 == stStat.u32LeftStreamFrames)
				 {						SAMPLE_PRT("NOTE: Current  frame is NULL!\n");
					continue;
				 }					
				 *******************************************************/
                if (0 == stStat.u32CurPacks)
                {
                    SAMPLE_PRT("NOTE: Current  frame is NULL!\n");
                    continue;
                }                    
                /*******************************************************
                 step 2.3 : malloc corresponding number of pack nodes.
                *******************************************************/
                stStream.pstPack = (VENC_PACK_S*)malloc(sizeof(VENC_PACK_S) * stStat.u32CurPacks);
                if (NULL == stStream.pstPack)
                {
                    SAMPLE_PRT("malloc stream pack failed!\n");
                    break;
                }                    
                /*******************************************************
                 step 2.4 : call mpi to get one-frame stream
                *******************************************************/
                stStream.u32PackCount = stStat.u32CurPacks;
                s32Ret = HI_MPI_VENC_GetStream(i, &stStream, HI_TRUE);
                if (HI_SUCCESS != s32Ret)
                {
                    free(stStream.pstPack);
                    stStream.pstPack = NULL;
                    SAMPLE_PRT("HI_MPI_VENC_GetStream failed with %#x!\n", \
                               s32Ret);
                    break;
                }                    
                /*******************************************************
                 step 2.5 : save frame to file
                *******************************************************/
  				s32Ret = Agora_VENC_SendStream(&stStream);
                if (HI_SUCCESS != s32Ret)
                {
                    free(stStream.pstPack);
                    stStream.pstPack = NULL;
                    SAMPLE_PRT("send Agora stream failed!\n");
                    break;
                }                    
                /*******************************************************
                 step 2.6 : release stream                    
                 *******************************************************/
                s32Ret = HI_MPI_VENC_ReleaseStream(i, &stStream);
                if (HI_SUCCESS != s32Ret)
                {
                    free(stStream.pstPack);
                    stStream.pstPack = NULL;
                    break;
                }                    
                /*******************************************************
                 step 2.7 : free pack nodes
                *******************************************************/
                free(stStream.pstPack);
                stStream.pstPack = NULL;
            }
        }
    }    

    return NULL;
}

HI_S32 Agora_Enc_StartGetStream(HI_S32 s32Cnt)
{
    gs_stPara.bThreadStart = HI_TRUE;
    gs_stPara.s32Cnt = s32Cnt;
    return pthread_create(&gs_VencPid, 0, Agora_Enc_GetVencStreamProc, (HI_VOID*)&gs_stPara);
}

HI_S32 Agora_Hi_SysInit(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;
    VB_CONF_S stVbConf;
	
    /******************************************
     step  : init sys variable 
    ******************************************/
    memset(&stVbConf,0,sizeof(VB_CONF_S));
	
    stVbConf.u32MaxPoolCnt = 128;
    /*video buffer*/ 
    u32BlkSize = Agora_Sys_CalcPicVbBlkSize(gs_enNorm,\
                enSize, PIXEL_FORMAT_YUV_SEMIPLANAR_420, 16,COMPRESS_MODE_SEG);
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = 32;


    /******************************************
     step : mpp system init. 
    ******************************************/
    s32Ret = MPP_Sys_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        AGORA_TEST_PRT("system init failed with %d!\n", s32Ret);
        return -1;
    }

	return HI_SUCCESS;

}


HI_S32 Agora_Hi_EncInit(HI_VOID)
{
    PAYLOAD_TYPE_E enPayLoad = PT_H264;
    PIC_SIZE_E enSize = PIC_HD1080;
	HI_U32 u32Profile = 0;
	
	SAMPLE_VI_MODE_E enViMode = SAMPLE_VI_MODE_8_1080P;
    HI_S32 s32VpssGrpCnt = 8;
    
    VPSS_GRP VpssGrp;
    VPSS_CHN VpssChn;
    VPSS_GRP_ATTR_S stVpssGrpAttr = {0};

    
    VENC_CHN VencChn;
    SAMPLE_RC_E enRcMode= SAMPLE_RC_CBR;
	
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BlkSize;
    SIZE_S stSize;
    char c;

    /******************************************
     step : start vi dev & chn to capture
    ******************************************/
    s32Ret = Agora_Vi_Capture(enViMode,gs_enNorm);
    if (HI_SUCCESS != s32Ret)
    {
        AGORA_TEST_PRT("start vi failed!\n");
        return -1;
    }
    
    /******************************************
     step : start vpss and vi bind vpss
    ******************************************/
    s32Ret = Agora_Sys_GetPicSize(gs_enNorm, enSize, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        return -1;
    }

    VpssGrp = 0;
   	memset(&stVpssGrpAttr,0,sizeof(VPSS_GRP_ATTR_S));
	stVpssGrpAttr.u32MaxW 	= stSize.u32Width;
	stVpssGrpAttr.u32MaxH 	= stSize.u32Height;
	stVpssGrpAttr.bNrEn 	= HI_TRUE;
    stVpssGrpAttr.enDieMode = VPSS_DIE_MODE_NODIE;
	stVpssGrpAttr.enPixFmt 	= SAMPLE_PIXEL_FORMAT;
	s32Ret = Agora_VPSS_Start(s32VpssGrpCnt, &stSize, 1, &stVpssGrpAttr);
    if (HI_SUCCESS != s32Ret)
    {
	    SAMPLE_PRT("Start Vpss failed!\n");
        return -1;
    }

    s32Ret = Agora_Vi_BindVpss();
    if (HI_SUCCESS != s32Ret)
    {
	    SAMPLE_PRT("Vi bind Vpss failed!\n");
        return -1;
    }


    /******************************************
     step : start stream venc
    ******************************************/
    /*** HD1080P **/

	VpssGrp = 0;
    VpssChn = 0;
    VencChn = 0;
	
    s32Ret = Agora_Enc_Start(VencChn, enPayLoad,\
                                   gs_enNorm, enSize, enRcMode,u32Profile);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start Venc failed!\n");
        return -1;
    }

    s32Ret = Agora_Enc_BindVpss(VencChn, VpssGrp, VpssChn);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start Venc failed!\n");
        return -1;
    }

  
    return HI_SUCCESS;    
}


HI_S32 Agora_Enc_SnapStart(VENC_CHN VencChn, SIZE_S* pstSize)
{
    HI_S32 s32Ret;
    VENC_CHN_ATTR_S stVencChnAttr;
    VENC_ATTR_JPEG_S stJpegAttr;
    stVencChnAttr.stVeAttr.enType = PT_JPEG;
    stJpegAttr.u32MaxPicWidth  = pstSize->u32Width;
    stJpegAttr.u32MaxPicHeight = pstSize->u32Height;
    stJpegAttr.u32PicWidth  = pstSize->u32Width;
    stJpegAttr.u32PicHeight = pstSize->u32Height;
    stJpegAttr.u32BufSize = pstSize->u32Width * pstSize->u32Height * 2;
    stJpegAttr.bByFrame = HI_TRUE;/*get stream mode is field mode  or frame mode*/
    stJpegAttr.bSupportDCF = HI_FALSE;
    memcpy(&stVencChnAttr.stVeAttr.stAttrJpege, &stJpegAttr, sizeof(VENC_ATTR_JPEG_S));
    s32Ret = HI_MPI_VENC_CreateChn(VencChn, &stVencChnAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VENC_CreateChn [%d] faild with %#x!\n", \
                   VencChn, s32Ret);
        return s32Ret;
    }
    return HI_SUCCESS;
}

HI_S32 Agora_Hi_PicInit(HI_VOID)
{
    HI_S32 s32Ret;
    HI_S32 VpssGrp = 0;
    HI_S32 VpssChn = 1;
    HI_S32 VencChn = 1;
	SIZE_S stSize;

	// 1080P 图像
	stSize.u32Height = 1080;
	stSize.u32Width = 1920;
	s32Ret = Agora_Enc_SnapStart(VencChn, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start snap failed!\n");
        return -1;
    }

	s32Ret = Agora_Enc_BindVpss(VencChn, VpssGrp, VpssChn);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start snap bind failed!\n");
        return -1;
    }

	return HI_SUCCESS;

}


HI_S32 Agora_Pic_SendSnap(VENC_STREAM_S* pstStream)
{
    HI_S32 s32Ret;

    s32Ret = agora_rtc_send_through_rdt(datachannel, pstStream->pstPack.pu8Addr, pstStream->pstPack.u32Len);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("send snap picture failed!\n");
        return HI_FAILURE;
    }
    gs_s32SnapCnt++;
    return HI_SUCCESS;
}


HI_S32  Agora_Pic_SnapProcess(HI_VOID)
{
    struct timeval TimeoutVal;
    fd_set read_fds;
    HI_S32 s32VencFd;
    VENC_CHN_STAT_S stStat;
    VENC_STREAM_S stStream;
    HI_S32 s32Ret;
    VENC_RECV_PIC_PARAM_S stRecvParam;
	VENC_STREAM_BUF_INFO_S stStreamBufInfo;
	VENC_CHN VencChn = 1;
		
    /******************************************
     step 2:  Start Recv Venc Pictures
    ******************************************/
    stRecvParam.s32RecvPicNum = 1;
    s32Ret = HI_MPI_VENC_StartRecvPicEx(VencChn, &stRecvParam);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VENC_StartRecvPic faild with%#x!\n", s32Ret);
        return HI_FAILURE;
    }    
    /******************************************
     step 3:  recv picture
    ******************************************/
    s32VencFd = HI_MPI_VENC_GetFd(VencChn);
    if (s32VencFd < 0)
    {
        SAMPLE_PRT("HI_MPI_VENC_GetFd faild with%#x!\n", s32VencFd);
        return HI_FAILURE;
    }
    FD_ZERO(&read_fds);
    FD_SET(s32VencFd, &read_fds);
    TimeoutVal.tv_sec  = 2;
    TimeoutVal.tv_usec = 0;

	{
	    s32Ret = select(s32VencFd + 1, &read_fds, NULL, NULL, &TimeoutVal);
	    if (s32Ret < 0)
	    {
	        SAMPLE_PRT("snap select failed!\n");
	        return ;
	    }
	    else if (0 == s32Ret)
	    {
	        SAMPLE_PRT("snap time out!\n");
	        continue;
	    }
	    else
	    {
	        if (FD_ISSET(s32VencFd, &read_fds))
	        {
	            s32Ret = HI_MPI_VENC_Query(VencChn, &stStat);
	            if (s32Ret != HI_SUCCESS)
	            {
	                SAMPLE_PRT("HI_MPI_VENC_Query failed with %#x!\n", s32Ret);
	                return HI_FAILURE;
	            }						
	            /*******************************************************
				suggest to check both u32CurPacks and u32LeftStreamFrames at the same time,for example:
				 if(0 == stStat.u32CurPacks || 0 == stStat.u32LeftStreamFrames)
				 {				SAMPLE_PRT("NOTE: Current  frame is NULL!\n");
					return HI_SUCCESS;
				 }			
				 *******************************************************/
	            if (0 == stStat.u32CurPacks)
	            {
	                SAMPLE_PRT("NOTE: Current  frame is NULL!\n");
	                return HI_SUCCESS;
	            }
	            stStream.pstPack = (VENC_PACK_S*)malloc(sizeof(VENC_PACK_S) * stStat.u32CurPacks);
	            if (NULL == stStream.pstPack)
	            {
	                SAMPLE_PRT("malloc memory failed!\n");
	                return HI_FAILURE;
	            }
	            stStream.u32PackCount = stStat.u32CurPacks;
	            s32Ret = HI_MPI_VENC_GetStream(VencChn, &stStream, -1);
	            if (HI_SUCCESS != s32Ret)
	            {
	                SAMPLE_PRT("HI_MPI_VENC_GetStream failed with %#x!\n", s32Ret);
	                free(stStream.pstPack);
	                stStream.pstPack = NULL;
	                return HI_FAILURE;
	            }
		        s32Ret = HI_MPI_VENC_GetStreamBufInfo (VencChn, &stStreamBufInfo);
		        if (HI_SUCCESS != s32Ret)
		        {
		            SAMPLE_PRT("HI_MPI_VENC_GetStreamBufInfo failed with %#x!\n", s32Ret);
	                free(stStream.pstPack);
	                stStream.pstPack = NULL;
		            return HI_FAILURE;
		        }
	            s32Ret = Agora_Pic_SendSnap(&stStream);
	            if (HI_SUCCESS != s32Ret)
	            {
	                SAMPLE_PRT("Agora_Pic_SendSnap failed with %#x!\n", s32Ret);
	                free(stStream.pstPack);
	                stStream.pstPack = NULL;
	                return HI_FAILURE;
	            }
	            s32Ret = HI_MPI_VENC_ReleaseStream(VencChn, &stStream);
	            if (s32Ret)
	            {
	                SAMPLE_PRT("HI_MPI_VENC_ReleaseStream failed with %#x!\n", s32Ret);
	                free(stStream.pstPack);
	                stStream.pstPack = NULL;
	                return HI_FAILURE;
	            }
	            free(stStream.pstPack);
	            stStream.pstPack = NULL;
	        }
	    }    		
	}
	return HI_SUCCESS;
}

HI_S32 Agora_Enc_StartGetSnap(HI_VOID)
{
    return pthread_create(&gs_VencPid, 0, Agora_Pic_SnapProcess, NULL;
}




typedef struct {
    const char  *p_sdk_log_dir;

    const char  *p_appid;
    const char  *p_token;
    const char  *p_channel;
    uint32_t    uid;

} app_config_t;
app_config_t	g_stAgoraConfig;
int32_t 		b_join_success_flag;

static void Agora_Hi_dchan_availability(const char *channel, int is_available)
{
    LOGD("%s ch=%s is_available=%d", TAG_EVENT, channel, is_available);
	if (is_available)
	{
		strncpy(datachannel, channel, 31);
	}
}

static void Agora_Hi_join_channel_success(const char *channel, int32_t elapsed)
{
    b_join_success_flag = 1;
    LOGI("%s join success, channel=%s elapsed=%d", TAG_EVENT, channel, elapsed);
}

static void Agora_Hi_on_cmd(const char *channel, uint32_t uid, int cmd, const void *param_ptr,
				   size_t param_len)
{
	HI_S32 sRet;
    /******************************************
     step : get picture, then send to agora data channel. 
     启动抓拍，获取图片并发送到数据通道
    ******************************************/
    sRet = Agora_Pic_SnapProcess();
    LOGI("Agora_Pic_SnapProcess, sRet=%d", sRet);
	return;
} 


static agora_rtc_event_handler_t event_handler = {
    .on_join_channel_success     = Agora_Hi_join_channel_success,
    .on_rdt_availability_changed = Agora_Hi_dchan_availability,
	.on_cmd 					 = Agora_Hi_on_cmd
};


HI_S32 Agora_Chn_Init(int32_t argc, char **argv)
{
	
	int32_t rval;

    SAMPLE_PRT("%s Welcome to RTSA SDK v%s", TAG_APP, agora_rtc_get_version());
	
    // 3. API: init agora rtc sdk
    int32_t appid_len = strlen(g_stAgoraConfig.p_appid);
    void *p_appid = (void *)(appid_len == 0 ? NULL : g_stAgoraConfig.p_appid);
    rval = agora_rtc_init(p_appid, g_stAgoraConfig.uid, &event_handler, g_stAgoraConfig.p_sdk_log_dir);
    if (rval < 0) {
        LOGE("%s agora sdk init failed, rval=%d error=%s", TAG_API, rval, agora_rtc_err_2_str(rval));
        return -1;
    }

    // 4. API: join channel
    int32_t token_len = strlen(g_stAgoraConfig.p_token);
    void *p_token = (void *)(token_len == 0 ? NULL : g_stAgoraConfig.p_token);

    rval = agora_rtc_join_channel(g_stAgoraConfig.p_channel, p_token, token_len);
    if (rval < 0) {
        LOGE("%s join channel %s failed, rval=%d error=%s", TAG_API, g_stAgoraConfig.p_channel, rval, agora_rtc_err_2_str(rval));
        return -1;
    }

    // 5. wait until join channel success or Ctrl-C trigger stop
    while (1) {
        if (b_join_success_flag) {
            break;
        }
        usleep(10000);
    }


    return rval;
}


HI_VOID main(HI_VOID)
{
	HI_S32 sChnCount = 1;
	HI_S32 s32Ret;

	// step: 海思系统初始化
	s32Ret = Agora_Hi_SysInit();
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("Sys Init failed!\n");
		return;
	}


    /******************************************
     step : agora init and join into the channel. 
     初始化RTC参数并加入通讯channel
    ******************************************/
	s32Ret = Agora_Chn_Init()
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("Agora_Chn_Init Init failed!\n");
		return;
	}
		

    /******************************************
     step : stream venc init &process -- create chn, get stream, then send to agora channel. 
     创建视频输入，编码参数，取视频流且传输到视频通道处理
    ******************************************/
	s32Ret = Agora_Hi_EncInit();
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("enc Init failed!\n");
		return;
	}

	s32Ret = Agora_Enc_StartGetStream(sChnCount);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("Start Venc failed!\n");
		return;
	}

    /******************************************
     step : stream pic init 初始化图片抓拍通道
    ******************************************/
	s32Ret = Agora_Hi_PicInit();
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("pic Init failed!\n");
		return;
	}
	
	while(1)
	{
		// TODO: recv the config cmd from tcplink

		// 释放CPU，休眠100ms，处理配置不需要那么快
		usleep(100000);
	}
	
}



