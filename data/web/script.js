function getFieldByAPI(url, fields) {
  fetch(url).then(r=>r.json()).then(r=>{
    for (const k in fields) {
      const kk = '#' + k;
      if ($(kk).attr('type') === 'checkbox') {
        $(kk).bootstrapSwitch('state', r[fields[k]])
      } else {
        $(kk).val(r[fields[k]])
      }
    }
  })
}

function postJSON(url, data, callback) {
  fetch(url, {
    method: 'POST',
    headers: {
      'Accept': 'application/json',
      'Content-Type': 'application/json'
    },
    body: JSON.stringify(data)
  }).then(r=>callback(r))
}

function setFieldByAPI(url, fields) {
  const data = {}
  for (const k in fields) {
    const kk = '#' + k;
    if ($(kk).attr('type') === 'checkbox') {
      data[fields[k]] = $(kk).prop('checked')
    } else {
      data[fields[k]] = $(kk).val()
    }
  }
  postJSON(url, data, (r)=>{})
}

function setTimeNow() {
  const seconds = Math.trunc(Date.now() / 1000);
  console.log("Current time:", seconds);
  postJSON('/api/time/sync', {now: seconds}, (r)=>{
    getTimeNow()
  })
}

function getTimeNow() {
  fetch('/api/time/now').then(r=>r.json()).then(r=>{
    const d = new Date(r.now * 1000)
    $('#disp-time-now').text(d.toString())
  })
}

const wifiStaMapping = {
  'sw-wifi-on': 'on',
  'input-wifi-ssid': 'ssid',
  'input-wifi-pwd': 'pwd'
}

const timeNtpMapping = {
  'sw-ntp-on': 'on',
  'input-time-ntp': 'server'
}

const deviceIdMapping = {'input-deviceid': 'id'}

function loadFieldValues() {
  getFieldByAPI('/api/wifi/sta', wifiStaMapping)
  getFieldByAPI('/api/time/ntp', timeNtpMapping)
  getFieldByAPI('/api/conf/id', deviceIdMapping)
}

function saveWifi(e) {
  e.preventDefault();
  setFieldByAPI('/api/wifi/sta', wifiStaMapping)
}

function saveTimeNtp(e) {
  e.preventDefault();
  setFieldByAPI('/api/time/ntp', timeNtpMapping)
}

function saveDeviceId(e) {
  e.preventDefault();
  setFieldByAPI('/api/conf/id', deviceIdMapping)
}

$(document).ready(function() {
  // Setup switch behavior
  $('#sw-wifi-on').change(saveWifi)
  $('#sw-ntp-on').change(saveTimeNtp)
  // Initialize switch
  $('#sw-wifi-on').bootstrapSwitch()
  $('#sw-ntp-on').bootstrapSwitch()
  // Setup save button behavior
  $('#btn-wifi-save').click(saveWifi)
  $('#btn-time-ntp-save').click(saveTimeNtp)
  $('#btn-deviceid-save').click(saveDeviceId)
  $('#btn-time-sync').click(setTimeNow)
  // Setup action button behavior
  $('.actionbtn').click(function(e) {
    e.preventDefault();
    fetch(this.data('url')).then(r=>loadFieldValues())
  })
  getTimeNow();
  // Load initial values
  loadFieldValues()
})