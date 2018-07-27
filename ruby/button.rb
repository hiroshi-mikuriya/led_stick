require './bcm2835'

if BCM.bcm2835_init.zero?
  puts 'failed to init bcm2835'
  exit 1
end

GPIO = 5

BCM.bcm2835_gpio_fsel(GPIO, 0)
BCM.bcm2835_gpio_set_pud(GPIO, 2)

loop { p BCM.bcm2835_gpio_lev(GPIO) }

