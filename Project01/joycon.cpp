#include "hidapi\hidapi.h"
#include "joycon.h"

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

// 説明
// windowsでは“Wireless Gamepad”と認識されます。
// ランプはビットフラグで、4桁。ランプの一番上から10進数で 1, 2, 4, 8 と対応しています。
// 参考URL: https://github.com/dekuNukem/Nintendo_Switch_Reverse_Engineering

#define JOYCON_L_PRODUCT_ID 0x2006 //8198
#define JOYCON_R_PRODUCT_ID 0x2007 //8199
#define VENDOR_ID  0x057E  // Nintendo Co., Ltd //0d1406

//====================================================================
//コンストラクタ
//====================================================================
CJoycon::CJoycon()
{
    // read input report
    uint8_t buff[0x40]; memset(buff, 0x00, size_t(0x40));
    // 読み込むサイズを指定。
    size_t size = sizeof(buff);
}

//====================================================================
//デストラクタ
//====================================================================
CJoycon::~CJoycon()
{

}

//====================================
// 見つけて接続します
//====================================
hid_device* CJoycon::open_joycon() {
    //struct hid_device_info* devs = hid_enumerate(0x057e, 0x2006); // Joy-ConのベンダーIDとプロダクトID
    struct hid_device_info* devs = hid_enumerate(0, 0); // Joy-ConのベンダーIDとプロダクトID
    hid_device* handle = nullptr;

    while (devs)
    {
        if (devs->product_id == JOYCON_L_PRODUCT_ID || devs->product_id == JOYCON_R_PRODUCT_ID)
        {
            // プロダクトID等を指定して、HID deviceをopenする。そうすると、そのhidデバイスの情報が載ったhid_deviceが帰ってくる。
            hid_device* handle = hid_open(devs->vendor_id, devs->product_id, devs->serial_number);
        }

        devs = devs->next;
    }

    exit(-1);

    hid_free_enumeration(devs);
    return handle;
}

//====================================
// Joyconに出力
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
// センサーを有効にする
//====================================
void CJoycon::enable_sensors(hid_device* handle) {
    uint8_t data[0x01];

    hid_set_nonblocking(dev, 0);

    // 0x30番のサブコマンドに、0x01を送信します。
    data[0] = 0x01; //Report ID
    SendSubcommand(dev, 0x30, data, 1, &globalCount);

    do
    {
        data[0] = 0x30; //command
        SendSubcommand(dev, 0x03, data, 1, &globalCount);

        data[0] = 0x01; //command
        SendSubcommand(dev, 0x40, data, 1, &globalCount);

        // input report を受けとる。
        int ret = hid_read(dev, buff, size);

    } while (*buff != 0x30 || buff[13] == 0);       //入力レポートのIDが48だったら、IMUデータが入っているか
}

//====================================
// IMUの更新処理
//====================================
void CJoycon::Sensor_Update() 
{
    int16_t accel[2];
    int16_t gyro[2];

    for (int i = 0; i < 2; i++)
    {
        //加速度を取得
        accel[i] = (buff[13 + 2 * i] | (buff[14 + 2 * i] << 8) & 0xFF00);

        //加速度を補正
        accel_correction[i] = (float)accel[i] * 0.000244f;
    }


    for (int i = 0; i < 2; i++)
    {
        //回転速度を取得
        gyro[i] = (buff[19 + 2 * i] | (buff[20 + 2 * i] << 8) & 0xFF00);

        //回転速度を補正
        gyro_radian[i] = (float)accel[i] * 0.070f;
    }
}

//====================================
// ジョイコンの更新処理
//====================================
void CJoycon::Update()
{
    // input report を受けとる。
    int ret = hid_read(dev, buff, size);

    // input report の id が 0x3F のものに絞る。
    if (*buff != 0x3F)
    {
        printf("\ninput report id: %d\n", *buff);

        if (*buff == 0x30)
        {
            Sensor_Update();
        }

        return;
    }

    //// input report の id　を表示
    //printf("\ninput report id: %d\n", *buff);
    //// ボタンのビットフィールドを表示
    //printf("button byte 1: %d\n", buff[1]);
    //printf("button byte 2: %d\n", buff[2]);
    //// スティックの状態を表示
    //printf("stick  byte 3: %d\n", buff[3]);

    //buffOld[1] = buff[1];
    //buffOld[2] = buff[2];
    //buffOld[3] = buff[3];

    //for (int i = 4; i < 64; i++)
    //{//探し用表示
    //    if (buffOld[i] != buff[i])
    //    {
    //        printf("IDX(%d): %d → %d\n", i, buffOld[i], buff[i]);
    //    }
    //    buffOld[i] = buff[i];
    //}

    //SendSubcommand(dev, 0x30, data, 1, &globalCount);
}






/*

//====================================
// 初期化
//====================================
void initialize_hidapi() {
    if (hid_init()) {
        std::cerr << "Failed to initialize HIDAPI." << std::endl;
        exit(-1);
    }
}

//====================================
// コマンドを送信
//====================================
void send_command(hid_device* handle, const std::vector<uint8_t>& command) {
    if (hid_write(handle, command.data(), command.size()) == -1) {
        std::cerr << "Failed to send command to Joy-Con." << std::endl;
        exit(-1);
    }
}

//====================================
// コマンドを送信
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
// センサーを有効にする
//====================================
void enable_sensors(hid_device* handle) {
    std::vector<uint8_t> enable_sensor_data = { 0x01, 0x40 };
    std::vector<uint8_t> enable_imu_data = { 0x01, 0x04, 0x01, 0x01 };

    send_command(handle, create_command(0x03, enable_sensor_data));
    send_command(handle, create_command(0x40, enable_imu_data));
}

//====================================
// センサー情報を表示
//====================================
void sensor_Update(hid_device* handle) {
    unsigned char buffer[49];
    int res;

    while (true) {
        res = hid_read(handle, buffer, sizeof(buffer));
        if (res > 0) {
            // センサーデータはバッファの18バイト目から始まる
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
// Joyconに出力
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
// メイン関数
//====================================
int main()
{
    int globalCount = 0;
    // 接続されているHIDデバイスの連結リストを取得。
    hid_device_info* device = hid_enumerate(0, 0);
    uint8_t buffOld[0x40]; memset(buffOld, 0x00, size_t(0x40));

    while (device)
    {
        if (device->product_id == JOYCON_L_PRODUCT_ID || device->product_id == JOYCON_R_PRODUCT_ID)
        {
            // プロダクトID等を指定して、HID deviceをopenする。そうすると、そのhidデバイスの情報が載ったhid_deviceが帰ってくる。
            hid_device* dev = hid_open(device->vendor_id, device->product_id, device->serial_number);
            // 今開いているデバイスのプロダクト名の取得。
            printf("\nproduct_id: %ls", device->product_string);

            // read input report
            uint8_t buff[0x40]; memset(buff, 0x00, size_t(0x40));
            // 読み込むサイズを指定。
            size_t size = sizeof(buff);

            uint8_t data[0x01];

            hid_set_nonblocking(dev, 0);

            // 0x30番のサブコマンドに、0x01を送信します。
            data[0] = 0x01; //Report ID
            SendSubcommand(dev, 0x30, data, 1, &globalCount);
            
            do
            {
                data[0] = 0x30; //command
                SendSubcommand(dev, 0x03, data, 1, &globalCount);

                data[0] = 0x01; //command
                SendSubcommand(dev, 0x40, data, 1, &globalCount);

                // input report を受けとる。
                int ret = hid_read(dev, buff, size);

            } while (*buff != 0x30 || buff[13] == 0);       //入力レポートのIDが48だったら、IMUデータが入っているか


            //対象デバイスの状態を取得し続ける
            while (true)
            {
                // input report を受けとる。
                int ret = hid_read(dev, buff, size);
                
                // input report の id が 0x3F のものに絞る。
                if (*buff != 0x3F)
                {
                    printf("\ninput report id: %d\n", *buff);

                    if (*buff == 0x30)
                    {
                        printf("button 3: %d\n", buff[3]);

                        //加速度を取得
                        int16_t accel_x = (buff[13] | (buff[14] << 8) & 0xFF00);
                        int16_t accel_y = (buff[15] | (buff[16] << 8) & 0xFF00);
                        int16_t accel_z = (buff[17] | (buff[18] << 8) & 0xFF00);

                        //加速度を補正
                        float accel_correction_x = (float)accel_x * 0.000244f;
                        float accel_correction_y = (float)accel_y * 0.000244f;
                        float accel_correction_z = (float)accel_z * 0.000244f;

                        printf("accelX:%.1f\n", accel_correction_x);
                        printf("accelY:%.1f\n", accel_correction_y);
                        printf("accelX:%.1f\n\n", accel_correction_z);

                        //std::cout << std::right << "Accel: (" << accel_correction_x << ", " << accel_correction_y << ", " << accel_correction_z << ")" << std::endl;
                        //printf("\n");

                        //回転速度を取得
                        int16_t gyro_x = (buff[19] | (buff[20] << 8) & 0xFF00);
                        int16_t gyro_y = (buff[21] | (buff[22] << 8) & 0xFF00);
                        int16_t gyro_z = (buff[23] | (buff[24] << 8) & 0xFF00);

                        //回転速度を補正
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
                        //{//探し用表示
                        //    if (buffOld[i] != buff[i])
                        //    {
                        //        printf("IDX(%d): %d → %d\n", i, buffOld[i], buff[i]);
                        //    }
                        //    buffOld[i] = buff[i];
                        //}
                    }
                
                    continue;
                }

                // input report の id　を表示
                printf("\ninput report id: %d\n", *buff);
                // ボタンのビットフィールドを表示
                printf("button byte 1: %d\n", buff[1]);
                printf("button byte 2: %d\n", buff[2]);
                // スティックの状態を表示
                printf("stick  byte 3: %d\n", buff[3]);

                buffOld[1] = buff[1];
                buffOld[2] = buff[2];
                buffOld[3] = buff[3];

                for (int i = 4; i < 64; i++)
                {//探し用表示
                    if (buffOld[i] != buff[i])
                    {
                        printf("IDX(%d): %d → %d\n", i, buffOld[i], buff[i]);
                    }
                    buffOld[i] = buff[i];
                }

                data[0] = buff[3];
                SendSubcommand(dev, 0x30, data, 1, &globalCount);
            }
        }
         //次のデバイスへ
        device = device->next;
    }
    hid_free_enumeration(device);
}
*/