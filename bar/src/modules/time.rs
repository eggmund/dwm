use super::Module;
use std::fmt;

#[derive(Default)]
pub struct TimeModule {
    time_string: String,
}

impl TimeModule {
    #[inline]
    fn get_datetime() -> String {
        let localtime: DateTime<Local> = Local::now();
        localtime.format("%A%v %I:%M%P").to_string()   // E.g: Sunday 02-01-2020 12:03pm
    }
}

impl Module for TimeModule {
    fn update(&mut self) {
        self.time_string = Self::get_datetime();
    }

    #[inline]
    fn needs_update(&self, update_counter: u64) -> bool {
        update_counter % config::TIME_UPDATE_PERIOD == 0
    }
}

impl fmt::Display for TimeModule {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.time_string)
    }
}
