#include "hidapi\hidapi.h"
#include "joycon.h"

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

// ����
// windows�ł́gWireless Gamepad�h�ƔF������܂��B
// �����v�̓r�b�g�t���O�ŁA4���B�����v�̈�ԏォ��10�i���� 1, 2, 4, 8 �ƑΉ����Ă��܂��B
// �Q�lURL: https://github.com/dekuNukem/Nintendo_Switch_Reverse_Engineering

#define JOYCON_L_PRODUCT_ID 0x2006 //8198
#define JOYCON_R_PRODUCT_ID 0x2007 //8199
#define VENDOR_ID  0x057E  // Nintendo Co., Ltd //0d1406

//====================================================================
//�R���X�g���N�^
//====================================================================
CJoycon::CJoycon()
{
    // read input report
    uint8_t buff[0x40]; memset(buff, 0x00, size_t(0x40));
    // �ǂݍ��ރT�C�Y���w��B
    size_t size = sizeof(buff);
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CJoycon::~CJoycon()
{

}

//====================================
// �����Đڑ����܂�
//====================================
hid_device* CJoycon::open_joycon() {
    //struct hid_device_info* devs = hid_enumerate(0x057e, 0x2006); // Joy-Con�̃x���_�[ID�ƃv���_�N�gID
    struct hid_device_info* devs = hid_enumerate(0, 0); // Joy-Con�̃x���_�[ID�ƃv���_�N�gID
    hid_device* handle = nullptr;

    while (devs)
    {
        if (devs->product_id == JOYCON_L_PRODUCT_ID || devs->product_id == JOYCON_R_PRODUCT_ID)
        {
            // �v���_�N�gID�����w�肵�āAHID device��open����B��������ƁA����hid�f�o�C�X�̏�񂪍ڂ���hid_device���A���Ă���B
            hid_device* handle = hid_open(devs->vendor_id, devs->product_id, devs->serial_number);
        }

        devs = devs->next;
    }

    exit(-1);

    hid_free_enumeration(devs);
    return handle;
}

//====================================
// Joycon�ɏo��
//====================================
void CJoycon::SendSubcommand(hid_device* dev, uint8_t command, uint8_t data[], int len, int* globalCount)
{
    uint8_t buf[0x40]; memset(buf, 0x0, size_t(0x40));

    buf[0] = 1; // 0x10 for rumble only
    buf[1] = *globalCount; // Increment by 1 for each packet sent. It loops in 0x0 - 0xF range.

    if (*globalCount == 0xf0) {
        *globalCount = 0x00;
    }
    else {
        *globalCount++;
    }

    buf[10] = command;
    memcpy(buf + 11, data, len);

    hid_write(dev, buf, 0x40);
}

//====================================
// �Z���T�[��L���ɂ���
//====================================
void CJoycon::enable_sensors(hid_device* handle) {
    uint8_t data[0x01];

    hid_set_nonblocking(dev, 0);

    // 0x30�Ԃ̃T�u�R�}���h�ɁA0x01�𑗐M���܂��B
    data[0] = 0x01; //Report ID
    SendSubcommand(dev, 0x30, data, 1, &globalCount);

    do
    {
        data[0] = 0x30; //command
        SendSubcommand(dev, 0x03, data, 1, &globalCount);

        data[0] = 0x01; //command
        SendSubcommand(dev, 0x40, data, 1, &globalCount);

        // input report ���󂯂Ƃ�B
        int ret = hid_read(dev, buff, size);

    } while (*buff != 0x30 || buff[13] == 0);       //���̓��|�[�g��ID��48��������AIMU�f�[�^�������Ă��邩
}

//====================================
// IMU�̍X�V����
//====================================
void CJoycon::Sensor_Update() 
{
    int16_t accel[2];
    int16_t gyro[2];

    for (int i = 0; i < 2; i++)
    {
        //�����x���擾
        accel[i] = (buff[13 + 2 * i] | (buff[14 + 2 * i] << 8) & 0xFF00);

        //�����x��␳
        accel_correction[i] = (float)accel[i] * 0.000244f;
    }


    for (int i = 0; i < 2; i++)
    {
        //��]���x���擾
        gyro[i] = (buff[19 + 2 * i] | (buff[20 + 2 * i] << 8) & 0xFF00);

        //��]���x��␳
        gyro_radian[i] = (float)accel[i] * 0.070f;
    }
}

//====================================
// �W���C�R���̍X�V����
//====================================
void CJoycon::Update()
{
    // input report ���󂯂Ƃ�B
    int ret = hid_read(dev, buff, size);

    // input report �� id �� 0x3F �̂��̂ɍi��B
    if (*buff != 0x3F)
    {
        printf("\ninput report id: %d\n", *buff);

        if (*buff == 0x30)
        {
            Sensor_Update();
        }

        return;
    }

    //// input report �� id�@��\��
    //printf("\ninput report id: %d\n", *buff);
    //// �{�^���̃r�b�g�t�B�[���h��\��
    //printf("button byte 1: %d\n", buff[1]);
    //printf("button byte 2: %d\n", buff[2]);
    //// �X�e�B�b�N�̏�Ԃ�\��
    //printf("stick  byte 3: %d\n", buff[3]);

    //buffOld[1] = buff[1];
    //buffOld[2] = buff[2];
    //buffOld[3] = buff[3];

    //for (int i = 4; i < 64; i++)
    //{//�T���p�\��
    //    if (buffOld[i] != buff[i])
    //    {
    //        printf("IDX(%d): %d �� %d\n", i, buffOld[i], buff[i]);
    //    }
    //    buffOld[i] = buff[i];
    //}

    //SendSubcommand(dev, 0x30, data, 1, &globalCount);
}






/*

//====================================
// ������
//====================================
void initialize_hidapi() {
    if (hid_init()) {
        std::cerr << "Failed to initialize HIDAPI." << std::endl;
        exit(-1);
    }
}

//====================================
// �R�}���h�𑗐M
//====================================
void send_command(hid_device* handle, const std::vector<uint8_t>& command) {
    if (hid_write(handle, command.data(), command.size()) == -1) {
        std::cerr << "Failed to send command to Joy-Con." << std::endl;
        exit(-1);
    }
}

//====================================
// �R�}���h�𑗐M
//====================================
std::vector<uint8_t> create_command(uint8_t command_id, const std::vector<uint8_t>& data) {
    std::vector<uint8_t> command(10 + data.size());
    command[0] = 0x01;
    command[1] = 0x00;
    command[2] = 0x00;
    command[3] = 0x00;
    command[4] = 0x00;
    command[5] = 0x00;
    command[6] = 0x00;
    command[7] = 0x00;
    command[8] = command_id;
    std::copy(data.begin(), data.end(), command.begin() + 9);
    return command;
}

//====================================
// �Z���T�[��L���ɂ���
//====================================
void enable_sensors(hid_device* handle) {
    std::vector<uint8_t> enable_sensor_data = { 0x01, 0x40 };
    std::vector<uint8_t> enable_imu_data = { 0x01, 0x04, 0x01, 0x01 };

    send_command(handle, create_command(0x03, enable_sensor_data));
    send_command(handle, create_command(0x40, enable_imu_data));
}

//====================================
// �Z���T�[����\��
//====================================
void sensor_Update(hid_device* handle) {
    unsigned char buffer[49];
    int res;

    while (true) {
        res = hid_read(handle, buffer, sizeof(buffer));
        if (res > 0) {
            // �Z���T�[�f�[�^�̓o�b�t�@��18�o�C�g�ڂ���n�܂�
            int16_t button = static_cast<int>(buffer[1]);
            int16_t button_sp = static_cast<int>(buffer[2]);
            int16_t stick = static_cast<int>(buffer[3]);

            int16_t accel_x = (buffer[19] << 8) | buffer[18];
            int16_t accel_y = (buffer[21] << 8) | buffer[20];
            int16_t accel_z = (buffer[23] << 8) | buffer[22];

            int16_t gyro_x = (buffer[25] << 8) | buffer[24];
            int16_t gyro_y = (buffer[27] << 8) | buffer[26];
            int16_t gyro_z = (buffer[29] << 8) | buffer[28];

            printf("\ninput report id:%d\n" ,*buffer);
            std::cout << "butNM byte:" << button << std::endl;
            std::cout << "butSP byte:" << button_sp << std::endl;
            std::cout << "stick byte:" << stick << std::endl;

            std::cout << "Accel: (" << accel_x << ", " << accel_y << ", " << accel_z << ") " << std::endl;
            std::cout << "Gyro: (" << gyro_x << ", " << gyro_y << ", " << gyro_z << ")" << std::endl;
        }
    }
}

*/




/*

//====================================
// Joycon�ɏo��
//====================================
void SendSubcommand(hid_device* dev, uint8_t command, uint8_t data[], int len, int* globalCount)
{
    uint8_t buf[0x40]; memset(buf, 0x0, size_t(0x40));

    buf[0] = 1; // 0x10 for rumble only
    buf[1] = *globalCount; // Increment by 1 for each packet sent. It loops in 0x0 - 0xF range.

    if (*globalCount == 0xf0) {
        *globalCount = 0x00;
    }
    else {
        *globalCount++;
    }

    buf[10] = command;
    memcpy(buf + 11, data, len);

    hid_write(dev, buf, 0x40);
}

//====================================
// ���C���֐�
//====================================
int main()
{
    int globalCount = 0;
    // �ڑ�����Ă���HID�f�o�C�X�̘A�����X�g���擾�B
    hid_device_info* device = hid_enumerate(0, 0);
    uint8_t buffOld[0x40]; memset(buffOld, 0x00, size_t(0x40));

    while (device)
    {
        if (device->product_id == JOYCON_L_PRODUCT_ID || device->product_id == JOYCON_R_PRODUCT_ID)
        {
            // �v���_�N�gID�����w�肵�āAHID device��open����B��������ƁA����hid�f�o�C�X�̏�񂪍ڂ���hid_device���A���Ă���B
            hid_device* dev = hid_open(device->vendor_id, device->product_id, device->serial_number);
            // ���J���Ă���f�o�C�X�̃v���_�N�g���̎擾�B
            printf("\nproduct_id: %ls", device->product_string);

            // read input report
            uint8_t buff[0x40]; memset(buff, 0x00, size_t(0x40));
            // �ǂݍ��ރT�C�Y���w��B
            size_t size = sizeof(buff);

            uint8_t data[0x01];

            hid_set_nonblocking(dev, 0);

            // 0x30�Ԃ̃T�u�R�}���h�ɁA0x01�𑗐M���܂��B
            data[0] = 0x01; //Report ID
            SendSubcommand(dev, 0x30, data, 1, &globalCount);
            
            do
            {
                data[0] = 0x30; //command
                SendSubcommand(dev, 0x03, data, 1, &globalCount);

                data[0] = 0x01; //command
                SendSubcommand(dev, 0x40, data, 1, &globalCount);

                // input report ���󂯂Ƃ�B
                int ret = hid_read(dev, buff, size);

            } while (*buff != 0x30 || buff[13] == 0);       //���̓��|�[�g��ID��48��������AIMU�f�[�^�������Ă��邩


            //�Ώۃf�o�C�X�̏�Ԃ��擾��������
            while (true)
            {
                // input report ���󂯂Ƃ�B
                int ret = hid_read(dev, buff, size);
                
                // input report �� id �� 0x3F �̂��̂ɍi��B
                if (*buff != 0x3F)
                {
                    printf("\ninput report id: %d\n", *buff);

                    if (*buff == 0x30)
                    {
                        printf("button 3: %d\n", buff[3]);

                        //�����x���擾
                        int16_t accel_x = (buff[13] | (buff[14] << 8) & 0xFF00);
                        int16_t accel_y = (buff[15] | (buff[16] << 8) & 0xFF00);
                        int16_t accel_z = (buff[17] | (buff[18] << 8) & 0xFF00);

                        //�����x��␳
                        float accel_correction_x = (float)accel_x * 0.000244f;
                        float accel_correction_y = (float)accel_y * 0.000244f;
                        float accel_correction_z = (float)accel_z * 0.000244f;

                        printf("accelX:%.1f\n", accel_correction_x);
                        printf("accelY:%.1f\n", accel_correction_y);
                        printf("accelX:%.1f\n\n", accel_correction_z);

                        //std::cout << std::right << "Accel: (" << accel_correction_x << ", " << accel_correction_y << ", " << accel_correction_z << ")" << std::endl;
                        //printf("\n");

                        //��]���x���擾
                        int16_t gyro_x = (buff[19] | (buff[20] << 8) & 0xFF00);
                        int16_t gyro_y = (buff[21] | (buff[22] << 8) & 0xFF00);
                        int16_t gyro_z = (buff[23] | (buff[24] << 8) & 0xFF00);

                        //��]���x��␳
                        float gyro_radian_x = (float)gyro_x * 0.070f;
                        float gyro_radian_y = (float)gyro_y * 0.070f;
                        float gyro_radian_z = (float)gyro_z * 0.070f;

                        //std::cout << "Gyro: (" << gyro_radian_x << ", " << gyro_radian_y << ", " << gyro_radian_z <<")" << std::endl;
                        //printf("\n");

                        printf("gyroX:%.1f\n", gyro_radian_x);
                        printf("gyroY:%.1f\n", gyro_radian_y);
                        printf("gyroZ:%.1f\n\n", gyro_radian_z);

                        system("cls");
                    
                        //for (int i = 0; i < sizeof(buff); i++)
                        //{//�T���p�\��
                        //    if (buffOld[i] != buff[i])
                        //    {
                        //        printf("IDX(%d): %d �� %d\n", i, buffOld[i], buff[i]);
                        //    }
                        //    buffOld[i] = buff[i];
                        //}
                    }
                
                    continue;
                }

                // input report �� id�@��\��
                printf("\ninput report id: %d\n", *buff);
                // �{�^���̃r�b�g�t�B�[���h��\��
                printf("button byte 1: %d\n", buff[1]);
                printf("button byte 2: %d\n", buff[2]);
                // �X�e�B�b�N�̏�Ԃ�\��
                printf("stick  byte 3: %d\n", buff[3]);

                buffOld[1] = buff[1];
                buffOld[2] = buff[2];
                buffOld[3] = buff[3];

                for (int i = 4; i < 64; i++)
                {//�T���p�\��
                    if (buffOld[i] != buff[i])
                    {
                        printf("IDX(%d): %d �� %d\n", i, buffOld[i], buff[i]);
                    }
                    buffOld[i] = buff[i];
                }

                data[0] = buff[3];
                SendSubcommand(dev, 0x30, data, 1, &globalCount);
            }
        }
         //���̃f�o�C�X��
        device = device->next;
    }
    hid_free_enumeration(device);
}
*/