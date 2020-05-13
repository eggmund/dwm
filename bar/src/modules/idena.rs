use idena_api::{IdenaAPI, IdenaError};

use crate::config;
use super::Module;

struct IdenaModule {
    api: IdenaAPI,
    balance: f64,
    output: String,
}

impl IdenaModule {
    pub fn new() -> Self {
        let api_key = Self::get_api_key(config::IDENA_HOST_URL);
        Self {
            api: IdenaAPI::new(&api_key, config::IDENA_HOST_URL),
            balance: 0.0,
            output: String::new(),
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

    #[inline]
    async fn get_dna_balance(&self) -> Result<f64, IdenaError> {
        let balance_json = self.api.balance(config::IDENA_ADDRESS).await?;

        Ok(
            balance_json["balance"].as_str().unwrap().parse::<f64>().unwrap() +
            balance_json["stake"].as_str().unwrap().parse::<f64>().unwrap()
        )
    }
}

impl Module for IdenaModule {
    async fn update(&mut self) -> Result<(), Box<dyn std::error::Error>> {
        self.balance = self.get_dna_balance()?.await;
        self.output = self.balance.to_string();
        Ok(())
    }

    fn output(&self) -> String {
        self.output
    }
}
