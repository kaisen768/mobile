# mobile
4G网卡模块拨号上网（HUAWEI ME909s Series LTE Module）

cmake .. -DCMAKE_C_COMPILER=arm-hisiv500-linux-gcc \
         -DCMAKE_PREFIX_PATH=/home/kaisen/common/opt/host-airlink \
         -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
         -DMOBILE_UV_STATIC=ON \
         -DMOBILE_MXML_STATIC=ON \
         -DMOBILE_USE_JEMALLOC=ON

cmake .. -DCMAKE_C_COMPILER=arm-hisiv510-linux-gcc \
         -DCMAKE_PREFIX_PATH=/home/kaisen/common/opt/host-ltelinkse \
         -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
         -DMOBILE_UV_STATIC=ON \
         -DMOBILE_MXML_STATIC=ON \
         -DMOBILE_USE_JEMALLOC=ON
