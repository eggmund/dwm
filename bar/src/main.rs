mod config;
mod modules;

use chrono::prelude::*;
use binance::api::*;
use binance::market::*;
use std::thread;
use std::time::Duration;
use std::fmt::Display;

use modules::Module;

struct Bar {
    modules: Vec<Box<dyn Module>>,
}

impl Bar {
    pub fn new(modules: Vec<Box<dyn Module>>) -> Self {
        Self {
            modules,
        }
    }
}


// #[inline]
// async fn get_dna_balance(api: &IdenaAPI) -> Result<f64, IdenaError> {
//     let balance_json = api.balance(config::IDENA_ADDRESS).await?;

//     Ok(
//         balance_json["balance"].as_str().unwrap().parse::<f64>().unwrap() +
//         balance_json["stake"].as_str().unwrap().parse::<f64>().unwrap()
//     )
// }

#[inline]
fn get_eth_price(market: &Market) -> Result<f64, Box<dyn std::error::Error>> {
    Ok(market.get_price("ETHUSDT")?.price)
}


// bar is a program that updates text for dwm status bar
// e.g:     $ETH: 203.43 | 403 DNA | Sunday 02-01-2020 12:03pm
#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    use std::process::Command;

    let base_sleep = Duration::new(config::BASE_UPDATE_PERIOD, 0);

    // let api_key = get_api_key(config::API_KEY_FILE);
    // let idena_api = IdenaAPI::new(&api_key, config::IDENA_HOST_URL);
    let binance_client = Market::new(None, None);

    let mut update_counter: u64 = 0;

    let mut datetime: String = String::new();
    // let mut dna_balance: f64 = 0.0;
    let mut eth_price: f64 = 0.0;

    loop {
        let mut has_updated = false;    // true if any field has updated

        // if update_counter % config::IDENA_UPDATE_PERIOD == 0 {
        //     dna_balance = get_dna_balance(&idena_api).await?;
        //     println!("Updating idena balance: {}", dna_balance);
        //     has_updated = true;
        // }

        if update_counter % config::TIME_UPDATE_PERIOD == 0 {
            datetime = get_datetime();
            println!("Updating time: {}", datetime);
            has_updated = true;
        }

        if update_counter % config::BINANCE_UPDATE_PERIOD == 0 {
            eth_price = get_eth_price(&binance_client)?;
            println!("Updating binance readings: {}", eth_price);
            has_updated = true;
        }
    
        if has_updated {    // Update bar string
            println!("Updating bar string.");

            let bar_string = format!("| {:.2} $ETH | {:.2} DNA | {} |", eth_price, dna_balance, datetime);
            println!("{}", bar_string);
            
            Command::new("xsetroot")
                .args(&["-name", &bar_string])
                .output()
                .expect("xsetroot command failed to start.");
        }

        update_counter += 1;
        thread::sleep(base_sleep);    // Sleep
    }
}