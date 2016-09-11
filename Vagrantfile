# vi: set ft=ruby :

VAGRANTFILE_API_VERSION = "2"

Vagrant.configure(VAGRANTFILE_API_VERSION) do |config|
  config.vm.box_check_update = true
  config.ssh.forward_agent = true

  if Vagrant.has_plugin? 'vagrant-cachier'
    config.cache.scope = :box
    config.cache.enable :apt
    config.cache.enable :apt_lists
  end

  config.vm.provider :virtualbox do |vb|
    vb.gui = false
    vb.memory = 1500 # 1.5 GiB
  end

  config.vm.provision :ansible do |a|
    a.verbose = 'v'
    a.playbook = 'ansible/site.yml'
    a.extra_vars = {
      ansible_ssh_user: :vagrant,
      compiler_family: ENV['CC'] || 'gcc',
      hosts: 'localhost'
    }
  end

  config.vm.define :target do |t|
    t.vm.box = 'ubuntu/trusty64'
  end

  config.vm.define :travis do |t|
    t.vm.box = 'ubuntu/precise64'
  end
end
