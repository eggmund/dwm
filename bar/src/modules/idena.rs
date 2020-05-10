use idena_api::{IdenaAPI, IdenaError};

use crate::config;
use super::Module;

struct IdenaModule {
    api: IdenaAPI,
    balance_val: f64,
}

impl IdenaModule {
    pub fn new() -> Self {
        let api_key = Self::get_api_key(config::IDENA_HOST_URL);
        Self {
            api: IdenaAPI::new(&api_key, config::IDENA_HOST_URL),
            balance_val: 0.0,
        }
    }

    #[inline]
    fn get_api_key(path: &str) -> String {
        use std::fs::File;
        use std::io::Read;

        let mut api_file = File::open(path).unwrap();
        let mut contents = String::new();
        api_file.read_to_string(&mut contents).unwrap();
        contents
    }
}

impl Module for IdenaModule {
    fn update(&mut self) {

    }

    fn get_
}
