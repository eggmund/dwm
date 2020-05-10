// Bar config
pub const API_KEY_FILE: &str = "/home/josh/.config/Idena/node/datadir/api.key";
pub const IDENA_HOST_URL: &str = "http://localhost:9009/";
pub const IDENA_ADDRESS: &str = "0xd5da967d65bcafa164c996fdf99834c650be1e38";

pub const BASE_UPDATE_PERIOD: u64 = 2;
// NOTE: These values have to have a non-zero value
// For an update period of T seconds, you do T/BASE_UPDATE_PERIOD
pub const IDENA_UPDATE_PERIOD: u64 = (10 * 60)/BASE_UPDATE_PERIOD; // seconds per update/base update period
pub const TIME_UPDATE_PERIOD: u64 = 60/BASE_UPDATE_PERIOD;
pub const BINANCE_UPDATE_PERIOD: u64 = 2/BASE_UPDATE_PERIOD;
